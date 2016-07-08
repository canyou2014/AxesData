package com.cloudminds.smartrobot.fragment;

import java.io.File;
import java.io.FilenameFilter;
import java.util.Map;


public class MyFileFilter implements FilenameFilter {
	private Map<String,String> map = FileUtil.getMap(MyConstans.RECORD_FILE_NAME);
	public boolean isZip(String file) {
	    if (file.endsWith(".zip") && !hasUpload(file)){
	      return true;   
	    }else{   
	      return false; 
	    }   
	  }   

	private boolean hasUpload(String file) {
		if(map == null) {
			return false;
		}
		if("1".equals(map.get(file))) {
			return true;
		} else {
			return false;
		}
	}

	@Override
	public boolean accept(File dir, String name) {
		return isZip(name);
	}

}
