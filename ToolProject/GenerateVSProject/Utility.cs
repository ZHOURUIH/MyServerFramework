using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

class Utility
{
	public static string getRelativePath(string basePath, string absolutePath)
	{
		if (string.IsNullOrEmpty(basePath))
		{
			return absolutePath;
		}
		if (string.IsNullOrEmpty(absolutePath))
		{
			return absolutePath;
		}
		if (basePath == absolutePath)
		{
			return "./";
		}

		Uri baseUri = new Uri(basePath);
		Uri absoluteUri = new Uri(absolutePath);
		Uri relativeUri = baseUri.MakeRelativeUri(absoluteUri);
		string relativePath = Uri.UnescapeDataString(relativeUri.ToString());

		return relativePath.Replace('/', Path.DirectorySeparatorChar);
	}
	// 目录是否存在,dir是绝对路径
	public static bool isDirExist(string dir)
	{
		if (dir.Length == 0 || dir == "./" || dir == "../")
		{
			return true;
		}
		validPath(ref dir);
		return Directory.Exists(dir);
	}
	public static void validPath(ref string path)
	{
		if (path.Length == 0)
		{
			return;
		}
		// 不以/结尾,则加上/
		if (path[path.Length - 1] != '/')
		{
			path += "/";
		}
	}
	// 查找指定目录下的所有文件,path为绝对路径
	public static void findFiles(string path, List<string> fileList, List<string> patterns = null, bool recursive = true)
	{
		validPath(ref path);
		if (!isDirExist(path))
		{
			return;
		}
		DirectoryInfo folder = new DirectoryInfo(path);
		FileInfo[] fileInfoList = folder.GetFiles();
		int fileCount = fileInfoList.Length;
		int patternCount = patterns != null ? patterns.Count : 0;
		for (int i = 0; i < fileCount; ++i)
		{
			string fileName = fileInfoList[i].Name;
			// 如果需要过滤后缀名,则判断后缀
			if (patternCount > 0)
			{
				for (int j = 0; j < patternCount; ++j)
				{
					if (fileName.EndsWith(patterns[j], StringComparison.OrdinalIgnoreCase))
					{
						fileList.Add(path + fileName);
					}
				}
			}
			// 不需要过滤,则直接放入列表
			else
			{
				fileList.Add(path + fileName);
			}
		}
		// 查找所有子目录
		if (recursive)
		{
			string[] dirs = Directory.GetDirectories(path);
			int count = dirs.Length;
			for (int i = 0; i < count; ++i)
			{
				findFiles(dirs[i], fileList, patterns, recursive);
			}
		}
	}
	// 得到指定目录下的所有第一级子目录,path为绝对路径
	public static bool findFolders(string path, List<string> dirList, List<string> excludeList = null, bool recursive = true)
	{
		validPath(ref path);
		if (!isDirExist(path))
		{
			return false;
		}
		string[] dirs = Directory.GetDirectories(path);
		int count = dirs.Length;
		for (int i = 0; i < count; ++i)
		{
			string dir = dirs[i];
			if (excludeList != null && excludeList.Count > 0)
			{
				if (excludeList.Contains(getFileName(dir)))
				{
					continue;
				}
			}
			dirList.Add(dir);
			if (recursive)
			{
				findFolders(dir, dirList, excludeList, recursive);
			}
		}
		return true;
	}
	public static string removeAll(string str, char key)
	{
		StringBuilder builder = new StringBuilder(str);
		for (int i = builder.Length - 1; i >= 0; --i)
		{
			// 判断是否是需要移除的字符
			if (builder[i] == key)
			{
				builder.Remove(i, 1);
			}
		}
		return builder.ToString();
	}
	public static int lastIndexOf(StringBuilder builder, char value, int startIndex = -1)
	{
		int length = builder.Length;
		if (startIndex < 0 || startIndex > length - 1)
		{
			startIndex = length - 1;
		}
		for (int i = startIndex; i >= 0; --i)
		{
			if (builder[i] == value)
			{
				return i;
			}
		}
		return -1;
	}
	// 得到文件路径
	public static string getFilePath(string fileName, bool keepEndSlash = false)
	{
		StringBuilder builder = new StringBuilder(fileName);
		builder.Replace('\\', '/');
		// 从倒数第二个开始,因为即使最后一个是/也需要忽略
		int lastPos = lastIndexOf(builder, '/', builder.Length - 2);
		if (lastPos < 0)
		{
			return "";
		}
		if (keepEndSlash)
		{
			builder.Remove(lastPos + 1, builder.Length - lastPos - 1);
		}
		else
		{
			builder.Remove(lastPos, builder.Length - lastPos);
		}
		return builder.ToString();
	}
	public static string getFileName(string str)
	{
		StringBuilder builder = new StringBuilder(str);
		builder.Replace('\\', '/');
		int dotPos = lastIndexOf(builder, '/');
		if (dotPos != -1)
		{
			builder.Remove(0, dotPos + 1);
		}
		return builder.ToString();
	}
	public static string rightToLeft(string path)
	{
		if (path == null)
		{
			return null;
		}
		return path.Replace('\\', '/');
	}
	public static XmlDocument openXML(string fileName)
	{
		if (!File.Exists(fileName))
		{
			Console.WriteLine("文件不存在:" + fileName);
			Console.Read();
			return null;
		}
		XmlDocument doc = new XmlDocument();
		XmlReaderSettings settings = new XmlReaderSettings();
		settings.IgnoreComments = true;
		using (XmlReader reader = XmlReader.Create(fileName, settings))
		{
			doc.Load(reader);
		}
		return doc;
	}
	// 从node中删除指定的deleteChildName,不过只删除有子节点的名字为childWithInnerChild的deleteChildName
	public static void DeleteChildWithInnerChild(XmlNode node, string deleteChildName, List<string> childWithInnerChild)
	{
		// 删除有ClInclude或者ClCompile的ItemGroup
		for (int i = 0; i < node.ChildNodes.Count; ++i)
		{
			XmlNode child = node.ChildNodes[i];
			if (child.Name != deleteChildName)
			{
				continue;
			}
			bool needDelete = false;
			int countInChild = child.ChildNodes.Count;
			for (int j = 0; j < countInChild; ++j)
			{
				XmlNode innerChild = child.ChildNodes[j];
				if (childWithInnerChild.Contains(innerChild.Name))
				{
					needDelete = true;
					break;
				}
			}
			if (needDelete)
			{
				node.RemoveChild(child);
				--i;
			}
		}
	}
}