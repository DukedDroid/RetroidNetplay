package org.retroidnetplay.browser;

import android.os.Bundle;

public class ROMDirActivity extends DirectoryActivity {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.setPathSettingKey("rgui_browser_directory");
		super.setIsDirectoryTarget(true);
		super.onCreate(savedInstanceState);
	}
}
