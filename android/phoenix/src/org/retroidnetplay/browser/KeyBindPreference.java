package org.retroidnetplay.browser;

import android.content.Context;
import android.preference.DialogPreference;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import org.retroidnetplay.R;

class KeyBindEditText extends EditText
{
	KeyBindPreference pref;
	public KeyBindEditText(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	public void setBoundPreference(KeyBindPreference pref)
	{
		this.pref = pref;
	}
	
	@Override
	public boolean onKeyPreIme(int keyCode, KeyEvent event)
	{
		return pref.onKey(null, event.getKeyCode(), event);
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		return pref.onKey(null, event.getKeyCode(), event);
	}
}

class KeyBindPreference extends DialogPreference implements View.OnKeyListener, AdapterView.OnItemClickListener, LayoutInflater.Factory {
	private int key_bind_code;
	private boolean grabKeyCode = false;
	KeyBindEditText keyText;
	private String[] key_labels;
	private final int DEFAULT_KEYCODE = 0;

	public KeyBindPreference(Context context, AttributeSet attrs) {
		super(context, attrs);
		key_labels = getContext().getResources().getStringArray(R.array.key_bind_values);
	}
	
	private void setKey(int keyCode, boolean force)
	{
		if (grabKeyCode || force)
		{
			grabKeyCode = false;
			key_bind_code = keyCode;
			keyText.setText("Current: " + key_labels[key_bind_code]);
		}
	}

	@Override
	protected void onBindDialogView(View view) {
	    super.onBindDialogView(view);
	}
	
	@Override
	protected View onCreateDialogView()
	{
		LayoutInflater inflater = ((LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE)).cloneInContext(getContext());
		inflater.setFactory(this);
		View view = inflater.inflate(R.layout.key_bind_dialog, null);
		keyText = (KeyBindEditText) view.findViewById(R.id.key_bind_value);
		keyText.setBoundPreference(this);
		view.setOnKeyListener(this);
		((ListView) view.findViewById(R.id.key_bind_list)).setOnItemClickListener(this);
		((Button) view.findViewById(R.id.key_bind_clear)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
        		setKey(0, true);
            }
        });
		((Button) view.findViewById(R.id.key_bind_detect)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	grabKeyCode = true;
    			keyText.setText("Press key to use");
    			keyText.requestFocus();
            }
        });
		InputMethodManager imm = (InputMethodManager) getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
		imm.hideSoftInputFromWindow(keyText.getWindowToken(), 0);
		setKey(getPersistedInt(DEFAULT_KEYCODE), true);
		return view;
	}

	@Override
	protected void onDialogClosed(boolean positiveResult) {
	   super.onDialogClosed(positiveResult);
	
	    if (positiveResult) {
	    	persistInt(key_bind_code);
	    	notifyChanged();
	    }
	}
	
	@Override
	public boolean onKey(View v, int keyCode, KeyEvent event) {
		if (grabKeyCode && event.getAction() == KeyEvent.ACTION_DOWN && keyCode != 0 && keyCode < key_labels.length)
		{
			setKey(keyCode, false);
			return true;
		}
		return false;
	}

	@Override
	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
		setKey((int)id, true);
	}

    @Override
    public CharSequence getSummary() {
    	int code = getPersistedInt(DEFAULT_KEYCODE);
		if (code >= key_labels.length)
			return "";
		else
			return key_labels[code];
    }
    
    @Override
    public View onCreateView(String name, Context context, AttributeSet attrs) {
    	Log.i("RetroArch", "view name: " + name);
    	if (name.equals("EditText"))
    		return new KeyBindEditText(context, attrs);
    	else
    		return null;
    }
}