package com.example.prakhar;

import android.app.Dialog;
import android.content.Context;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class AlertDialog extends Dialog {

    private TextView alertTitle;
    private TextView alertMessage;
    private Button alertOkButton;

    public AlertDialog( Context context, String title, String message

    ) {
        super(context);
        setContentView(R.layout.alert_dialog);


        alertTitle = (TextView) findViewById(R.id.alertTitle);
        alertMessage = (TextView) findViewById(R.id.alertMessage);
        alertOkButton = (Button) findViewById(R.id.alertOkButton);

        alertTitle.setText(title);
        alertMessage.setText(message);

        alertOkButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                dismiss();
            }
        });
    }
}

