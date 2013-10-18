package org.retroidnetplay.browser;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.provider.Settings;

public class RetroTVMode extends Activity {
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		MainMenuActivity.getInstance().updateConfigFile();
		
		Intent myIntent = new Intent(this, RetroActivity.class);
		String current_ime = Settings.Secure.getString(getContentResolver(), Settings.Secure.DEFAULT_INPUT_METHOD);
		myIntent.putExtra("CONFIGFILE", MainMenuActivity.getDefaultConfigPath());
		myIntent.putExtra("IME", current_ime);
		startActivity(myIntent);
		finish();
	}
}


