package com.cloudminds.smartrobot.fragment;

import java.io.File;
import java.io.FilenameFilter;

public class MyDeleteFileFilter implements FilenameFilter {
	public boolean isZip(String file) {
		if (file.endsWith(".zip")) {
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
