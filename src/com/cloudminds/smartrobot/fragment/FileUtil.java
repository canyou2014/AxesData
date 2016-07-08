package com.cloudminds.smartrobot.fragment;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public class FileUtil {

	public static boolean saveJson(String fileName, String content) {
		File file = new File(fileName);
		FileOutputStream os;
		try {
			os = new FileOutputStream(file);
			os.write(content.getBytes());
			os.close();
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	public static String readJson(String fileName) {
		String content = "";
		File file = new File(fileName);
		try {
			if (!file.exists()) {
				return null;
			}
			FileInputStream is = new FileInputStream(file);
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			byte[] buffer = new byte[1024];
			int length = -1;
			while ((length = is.read(buffer)) != -1) {
				stream.write(buffer, 0, length);
			}
			content = stream.toString();
			stream.close();
			is.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return null;
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}
		return content;
	}

	public static boolean savaZip(String fileName, String content) {
		try {
			File file = new File(fileName);
			InputStream input = getStringInputStream(content);
			ZipOutputStream zipOut = null;
			zipOut = new ZipOutputStream(new FileOutputStream(file));
			zipOut.putNextEntry(new ZipEntry(file.getName().replace("zip", "csv")));
			int temp = 0;
			while ((temp = input.read()) != -1) {
				zipOut.write(temp);
			}
			input.close();
			zipOut.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return false;
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	public static InputStream getStringInputStream(String content) {
		if (content != null && !content.equals("")) {
			try {
				ByteArrayInputStream stringInputStream = new ByteArrayInputStream(content.getBytes());
				return stringInputStream;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return null;
	}
	
	public static void saveData(String fileName, String content) {
		File file = new File(fileName);
		FileOutputStream os;
		try {
			os = new FileOutputStream(file,true);
			os.write(content.getBytes());
			os.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public static String[] getFilePaths(String dir) {
		File file = new File(dir);
		String[] fileNames = file.list(new MyFileFilter());
		return fileNames;
	}
	
	public static Map<String,String> getMap(String recordFileName) {
		Map<String,String> map = new HashMap<String,String>();
		String content = readJson(recordFileName);
		if (content == null) {
			return null;
		}
		String[] item = content.split("\n");
		int lenght = item.length;
		for(int i = 0; i < lenght; i++) {
			String[] value = item[i].split("\t");
			map.put(value[0], value[1]);
		}
		return map;
	}
	
	public static int deleteLastFile(String dir) {
		try {
			File file = new File(dir);
			File[] files = file.listFiles(new MyDeleteFileFilter());
			int len = files.length;
			if(len == 0) {
				return 0;
			}
			int index = 0;
			long temp = files[0].lastModified();
			for(int i = 1; i < len; i++) {
				if(files[i].lastModified() > temp) {
					index = i;
					temp = files[i].lastModified();
				}
			}
			files[index].delete();
		} catch (Exception e) {
			e.printStackTrace();
			return 1;
		}
		return 2;
	}

}
