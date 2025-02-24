using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;
using static Utility;

class Program
{
	static string replaceVariable(Dictionary<string, string> variableDefine, string value)
	{
		foreach (var variable in variableDefine)
		{
			if (value.Contains("{" + variable.Key + "}"))
			{
				value = value.Replace("{" + variable.Key + "}", variable.Value);
			}
		}
		return value;
	}
	static void Main(string[] args)
	{
		byte[] bytes = null;
		try
		{
			using (FileStream configFile = new FileStream("./GenerateVSProject_Config.txt", FileMode.Open))
			{
				bytes = new byte[configFile.Length];
				configFile.Read(bytes, 0, (int)configFile.Length);
			}
		}
		catch
		{
			Console.WriteLine("找不到配置文件:GenerateVSProject_Config.txt");
			Console.Read();
			return;
		}
		Dictionary<string, string> variableDefine = new Dictionary<string, string>();
		string[] lines = removeAll(Encoding.UTF8.GetString(bytes), '\r').Split('\n');
		foreach (string line in lines)
		{
			if (line[0] == '[' && line[line.Length - 1] == ']')
			{
				break;
			}
			if (line.Contains("="))
			{
				string[] tempPair = line.Split('=');
				if (tempPair.Length != 2)
				{
					Console.WriteLine(line + "配置错误");
					Console.Read();
					return;
				}
				// 检查是否有引用之前的变量定义
				variableDefine.Add(tempPair[0], rightToLeft(replaceVariable(variableDefine, tempPair[1])));
			}
		}
		lines[0] = null;

		Dictionary<string, Dictionary<string, string>> paramMap = new Dictionary<string, Dictionary<string, string>>();
		string curFlag = null;
		foreach (string line in lines)
		{
			if (line == null || line.Length == 0)
			{
				continue;
			}
			if (line[0] == '[' && line[line.Length - 1] == ']')
			{
				curFlag = line;
				paramMap.Add(curFlag, new Dictionary<string, string>());
				continue;
			}
			if (curFlag == null)
			{
				continue;
			}
			string[] paramPair = line.Split('=');
			if (paramPair.Length != 2)
			{
				continue;
			}
			// 将RootPath替换为配置的值
			paramMap[curFlag].Add(paramPair[0], rightToLeft(replaceVariable(variableDefine, paramPair[1])));
		}
		foreach (var item in paramMap.Values)
		{
			if (!item.TryGetValue("ProjectFilePath", out string projectFilePath))
			{
				Console.WriteLine("配置文件错误");
				Console.Read();
				return;
			}
			item.TryGetValue("ProjectIncludePath", out string includePath);
			item.TryGetValue("ExcludePath", out string excludePath);
			generateVCXProject(projectFilePath, includePath, new List<string>(excludePath.Split(',')));
			generateVCXProjectFilters(projectFilePath + ".filters");
		}
	}
	// projectPath需要是绝对路径
	protected static void generateVCXProject(string fileName, string includePath, List<string> excludePath)
	{
		if (!fileName.EndsWith(".vcxproj"))
		{
			Console.WriteLine("项目文件名错误:" + fileName);
			Console.Read();
			return;
		}

		XmlDocument doc = openXML(fileName);
		XmlNamespaceManager nsMgr = new XmlNamespaceManager(doc.NameTable);
		nsMgr.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003");

		XmlNode itemGroupNode = doc.SelectSingleNode("/ns:Project", nsMgr);
		// 删除有ClInclude或者ClCompile的ItemGroup
		DeleteChildWithInnerChild(itemGroupNode, "ItemGroup", new List<string>() { "ClInclude", "ClCompile" });

		XmlNode newNode = doc.CreateElement("ItemGroup", doc.DocumentElement.NamespaceURI);
		// 查找当前目录中的所有代码文件.h .cpp .c .inl
		List<string> sourceFiles = new List<string>();
		string projectPath = getFilePath(fileName) + "/";
		findFiles(projectPath, sourceFiles, new List<string>() { ".h", ".cpp", ".c", ".inl" });
		foreach (string source in sourceFiles)
		{
			// 只编译.c和UnityBuild.cpp文件
			string fileNameNoDir = getFileName(source);
			if (fileNameNoDir.EndsWith(".c") || (fileNameNoDir.StartsWith("UnityBuild") && fileNameNoDir.EndsWith(".cpp")))
			{
				XmlElement newChild = doc.CreateElement("ClCompile", doc.DocumentElement.NamespaceURI);
				newChild.SetAttribute("Include", source.Remove(0, projectPath.Length).Replace('/', '\\'));
				newNode.AppendChild(newChild);
			}
			else
			{
				XmlElement newChild = doc.CreateElement("ClInclude", doc.DocumentElement.NamespaceURI);
				newChild.SetAttribute("Include", source.Remove(0, projectPath.Length).Replace('/', '\\'));
				newNode.AppendChild(newChild);

				// inl文件还有一个子节点
				if (fileNameNoDir.EndsWith(".inl"))
				{
					XmlElement filterType = doc.CreateElement("FileType", doc.DocumentElement.NamespaceURI);
					filterType.InnerText = "Document";
					newChild.AppendChild(filterType);
				}
			}
		}
		if (newNode.ChildNodes.Count > 0)
		{
			itemGroupNode.AppendChild(newNode);
		}

		// 解析附加包含目录
		string allIncludePath = "";
		foreach (string item in includePath.Split(','))
		{
			if (item[item.Length - 1] == '*')
			{
				// 去除/*
				string thisPath = item.Substring(0, item.Length - 1);
				allIncludePath += getRelativePath(projectPath, thisPath) + ";";
				List<string> folders = new List<string>();
				findFolders(thisPath, folders);
				foreach (string folder in folders)
				{
					if (excludePath.Contains(folder))
					{
						continue;
					}
					List<string> tempFileList = new List<string>();
					findFiles(folder, tempFileList, new List<string>() { ".h", ".cpp", ".c", ".inl" });
					if (tempFileList.Count == 0)
					{
						continue;
					}
					allIncludePath += getRelativePath(projectPath, folder) + ";";
				}
			}
			else
			{
				allIncludePath += item + ";";
			}
		}
		allIncludePath = allIncludePath.Replace('\\', '/');
		for (int i = 0; i < itemGroupNode.ChildNodes.Count; ++i)
		{
			XmlNode child = itemGroupNode.ChildNodes[i];
			if (child.Name != "ItemDefinitionGroup")
			{
				continue;
			}
			XmlNode nodeCompile = child.SelectSingleNode("ns:ClCompile", nsMgr);
			if (nodeCompile == null)
			{
				continue;
			}
			XmlNode nodeInclude = nodeCompile.SelectSingleNode("ns:AdditionalIncludeDirectories", nsMgr);
			if (nodeInclude == null)
			{
				nodeInclude = doc.CreateElement("AdditionalIncludeDirectories", doc.DocumentElement.NamespaceURI);
				nodeCompile.AppendChild(nodeInclude);
			}
			nodeInclude.InnerText = allIncludePath;
		}
		doc.Save(fileName);
	}
	protected static XmlNode findChild(XmlNode node, string name)
	{
		for (int i = 0; i < node.ChildNodes.Count; ++i)
		{
			XmlNode child = node.ChildNodes[i];
			if (child.Name == name)
			{
				return child;
			}
		}
		return null;
	}
	protected static void generateVCXProjectFilters(string fileName)
	{
		XmlDocument doc = openXML(fileName);
		XmlNamespaceManager nsMgr = new XmlNamespaceManager(doc.NameTable);
		nsMgr.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003");

		XmlNode itemGroupNode = doc.SelectSingleNode("/ns:Project", nsMgr);
		Dictionary<string, string> filterGuid = new Dictionary<string, string>();
		// 找到原来的filter的Guid
		for (int i = 0; i < itemGroupNode.ChildNodes.Count; ++i)
		{
			var child = itemGroupNode.ChildNodes[i] as XmlElement;
			if (child.Name != "ItemGroup")
			{
				continue;
			}
			int countInChild = child.ChildNodes.Count;
			for (int j = 0; j < countInChild; ++j)
			{
				var innerChild = child.ChildNodes[j] as XmlElement;
				if (innerChild.Name == "Filter")
				{
					XmlNode id = innerChild.SelectSingleNode("ns:UniqueIdentifier", nsMgr);
					if (id.InnerText[0] != '{')
					{
						filterGuid.Add(innerChild.GetAttribute("Include"), "{" + id.InnerText + "}");
					}
					else
					{
						filterGuid.Add(innerChild.GetAttribute("Include"), id.InnerText);
					}
				}
			}
		}

		// 删除有ClInclude或者Filter的ItemGroup
		DeleteChildWithInnerChild(itemGroupNode, "ItemGroup", new List<string>() { "ClInclude", "Filter", "ClCompile" });

		XmlElement filterItemGroup = doc.CreateElement("ItemGroup", doc.DocumentElement.NamespaceURI);
		string projectPath = getFilePath(fileName) + "/";
		List<string> sourceDirs = new List<string>();
		findFolders(projectPath, sourceDirs);
		foreach (string dir in sourceDirs)
		{
			List<string> tempFileList = new List<string>();
			findFiles(dir, tempFileList, new List<string>() { ".h", ".cpp", ".c", ".inl" });
			if (tempFileList.Count == 0)
			{
				continue;
			}
			XmlElement filterNode = doc.CreateElement("Filter", doc.DocumentElement.NamespaceURI);
			string filterPath = dir.Remove(0, projectPath.Length).Replace('/', '\\');
			filterNode.SetAttribute("Include", filterPath);
			XmlElement identifierNode = doc.CreateElement("UniqueIdentifier", doc.DocumentElement.NamespaceURI);
			if (filterGuid.TryGetValue(filterPath, out string guid))
			{
				identifierNode.InnerText = guid;
			}
			else
			{
				identifierNode.InnerText = "{" + Guid.NewGuid().ToString() + "}";
			}
			filterNode.AppendChild(identifierNode);
			filterItemGroup.AppendChild(filterNode);
		}
		if (filterItemGroup.ChildNodes.Count > 0)
		{
			itemGroupNode.AppendChild(filterItemGroup);
		}

		XmlElement includeItemGroup = doc.CreateElement("ItemGroup", doc.DocumentElement.NamespaceURI);
		List<string> fileList = new List<string>();
		findFiles(projectPath, fileList, new List<string>() { ".h", ".cpp", ".c", ".inl" });
		foreach (string file in fileList)
		{
			// 只编译.c和UnityBuild.cpp文件,其他的都是include
			string fileNameNoDir = getFileName(file);
			if (!fileNameNoDir.EndsWith(".c") && (!fileNameNoDir.StartsWith("UnityBuild") || !fileNameNoDir.EndsWith(".cpp")))
			{
				XmlElement includeNode = doc.CreateElement("ClInclude", doc.DocumentElement.NamespaceURI);
				string relativeFileName = file.Remove(0, projectPath.Length).Replace('/', '\\');
				includeNode.SetAttribute("Include", relativeFileName);
				//string filter = getFilePath(relativeFileName);
				//if (filter != null && filter.Length > 0)
				//{
				//	XmlElement filterNode = doc.CreateElement("Filter", doc.DocumentElement.NamespaceURI);
				//	filterNode.InnerText = filter.Replace('/', '\\');
				//	includeNode.AppendChild(filterNode);
				//}
				includeItemGroup.AppendChild(includeNode);
			}
		}
		if (includeItemGroup.ChildNodes.Count > 0)
		{
			itemGroupNode.AppendChild(includeItemGroup);
		}

		XmlElement compileItemGroup = doc.CreateElement("ItemGroup", doc.DocumentElement.NamespaceURI);
		foreach (string file in fileList)
		{
			// 只编译.c和UnityBuild.cpp文件
			string fileNameNoDir = getFileName(file);
			if (fileNameNoDir.EndsWith(".c") || (fileNameNoDir.StartsWith("UnityBuild") && fileNameNoDir.EndsWith(".cpp")))
			{
				XmlElement conpileNode = doc.CreateElement("ClCompile", doc.DocumentElement.NamespaceURI);
				string relativeFileName = file.Remove(0, projectPath.Length).Replace('/', '\\');
				conpileNode.SetAttribute("Include", relativeFileName);
				//string filter = getFilePath(relativeFileName);
				//if (filter != null && filter.Length > 0)
				//{
				//	XmlElement filterNode = doc.CreateElement("Filter", doc.DocumentElement.NamespaceURI);
				//	filterNode.InnerText = filter.Replace('/', '\\');
				//	conpileNode.AppendChild(filterNode);
				//}
				compileItemGroup.AppendChild(conpileNode);
			}
		}
		if (compileItemGroup.ChildNodes.Count > 0)
		{
			itemGroupNode.AppendChild(compileItemGroup);
		}

		doc.Save(fileName);
	}
}