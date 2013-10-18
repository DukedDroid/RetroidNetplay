package org.retroidnetplay.browser;

import android.os.Bundle;

public class StateDirActivity extends DirectoryActivity {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.setPathSettingKey("savestate_directory");
		super.setIsDirectoryTarget(true);
		super.onCreate(savedInstanceState);
	}
}
