package com.example.prakhar;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Vibrator;
import android.view.WindowManager;
import android.widget.Toast;
import android.widget.TextView;

public class ReceiveSms extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent.getAction().equals("android.provider.Telephony.SMS_RECEIVED")) {
            Toast.makeText(context, "ACCIDENT DETECTED", Toast.LENGTH_SHORT).show();
            Uri notification = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE);
            Ringtone r = RingtoneManager.getRingtone(context, notification);
            r.play();

//            TEXT VIEW
//            TextView textView = new TextView(context);
//            textView.setText("ACCIDENT DETECTED");
//            textView.setTextSize(20);
//            textView.setTextColor(Color.RED);
//            textView.setPadding(16, 16, 16, 16);
//            WindowManager.LayoutParams params = new WindowManager.LayoutParams(
//                    WindowManager.LayoutParams.WRAP_CONTENT,
//                    WindowManager.LayoutParams.WRAP_CONTENT,
//                    WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
//                    WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
//                    PixelFormat.TRANSLUCENT);
//            WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
//            windowManager.addView(textView, params);
//
//            // Display alert dialog
//            AlertDialog alertDialog = new AlertDialog(context.getApplicationContext(), "New Message", "A new message has been received.");
//            alertDialog.show();

        }
        }
    }

