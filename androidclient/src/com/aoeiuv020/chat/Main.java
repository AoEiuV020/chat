package com.aoeiuv020.chat;

import android.app.Activity;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.Button;
import android.widget.Toast;
import android.widget.LinearLayout;
import android.view.View;

public class Main extends Activity
{
    /** Called when the activity is first created. */
	EditText eip,eport,esendtext;
	Button bconnect,bsendbutton;
	LinearLayout smessage;
	String message,ip;
	int port;
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
		try
		{
			super.onCreate(savedInstanceState);
			setContentView(R.layout.main);

			eip=(EditText)findViewById(R.id.ip);
			eport=(EditText)findViewById(R.id.port);
			esendtext=(EditText)findViewById(R.id.sendtext);
			bsendbutton=(Button)findViewById(R.id.sendbutton);
			bconnect=(Button)findViewById(R.id.connect);
			smessage=(LinearLayout)findViewById(R.id.messageview);

			bconnect.setOnClickListener(new View.OnClickListener() {
				public void onClick(View v)
				{
					try
					{
						ip=eip.getText().toString();
						port=Integer.parseInt(eport.getText().toString());
						int err;
						err=connect(ip,port);
						toast(errtostr(err));
					}
					catch(Exception e)
					{
						toast(e.toString());
					}
				}
				void toast(String str)
				{
					Toast.makeText(getApplicationContext(),str,Toast.LENGTH_SHORT).show();
				}
			});
			bsendbutton.setOnClickListener(new View.OnClickListener() {
				public void onClick(View v)
				{
					try
					{
						message=esendtext.getText().toString();
						int err;
						err=send(message);
						if(err!=0)
						{
							toast(errtostr(err));
						}
						else
						{
							addmessage(message);
							esendtext.setText("");
						}
					}
					catch(Exception e)
					{
						toast(e.toString());
					}
				}
				void toast(String str)
				{
					Toast.makeText(getApplicationContext(),str,Toast.LENGTH_SHORT).show();
				}
			});
			init();
		}
		catch(Exception e)
		{
			toast(e.toString());
		}
	}
	public void toast(String str)
	{
		Toast.makeText(getApplicationContext(),str,Toast.LENGTH_SHORT).show();
	}
	public void addmessage(String message)
	{
		EditText edittext;
		edittext=new EditText(getApplicationContext());
		edittext.setText(message);
		smessage.addView(edittext);
	}
	public native int connect(String ip,int port);
	public native int send(String message);
	public native int init();
	public native String errtostr(int err);
	static
	{
		System.loadLibrary("chat-jni");
	}
}
