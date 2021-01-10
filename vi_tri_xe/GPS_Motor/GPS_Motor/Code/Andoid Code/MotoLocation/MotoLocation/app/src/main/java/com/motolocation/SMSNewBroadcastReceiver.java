package com.motolocation;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.provider.Telephony;
import android.telephony.SmsMessage;
import android.util.Log;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

import java.util.List;

public class SMSNewBroadcastReceiver extends BroadcastReceiver {
    private String ACTION_SMS_NEW = "android.provider.Telephony.SMS_RECEIVED";
    private String TAG = SMSNewBroadcastReceiver.class.getName();
    private GoogleMap googleMap;
    private String phoneNumber;
    private LocationDatabaseHelper database;

    public SMSNewBroadcastReceiver(Context context,String number, GoogleMap map){
        this.googleMap = map;
        this.phoneNumber = number;
        googleMap.clear();
        database = new LocationDatabaseHelper(context);
        List<Location> locations = database.getLocations(phoneNumber);
        int size;
        if(locations.size() > 10){
            size = 9;
        }else size = locations.size();
        if(locations.size() != 0) {
            for (int i = 0; i < size; i++) {
                Location loca = locations.get(i);
                setMotorPosition(loca.getLat(), loca.getLng());
            }
        }
    }
    @Override
    public void onReceive(Context context, Intent intent) {
        Log.e(TAG, "Sms new");
        if(intent.getAction().equalsIgnoreCase(ACTION_SMS_NEW)){
            Log.e(TAG, "Sms new 1");
            String smsSender = "";
            String smsBody = "";
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
                for (SmsMessage smsMessage : Telephony.Sms.Intents.getMessagesFromIntent(intent)) {
                    smsSender = smsMessage.getDisplayOriginatingAddress();
                    smsBody += smsMessage.getMessageBody();
                }
            } else {
                Bundle smsBundle = intent.getExtras();
                if (smsBundle != null) {
                    Object[] pdus = (Object[]) smsBundle.get("pdus");
                    if (pdus == null) {
                        // Display some error to the user
                        Log.e(TAG, "SmsBundle had no pdus key");
                        return;
                    }
                    SmsMessage[] messages = new SmsMessage[pdus.length];
                    for (int i = 0; i < messages.length; i++) {
                        messages[i] = SmsMessage.createFromPdu((byte[]) pdus[i]);
                        smsBody += messages[i].getMessageBody();
                    }
                    smsSender = messages[0].getOriginatingAddress();
                }
            }
            Log.d("SMS", smsBody + smsSender);

            if (smsSender.replace("+84","0").equals(phoneNumber)) {
                if(smsBody != null && smsBody.contains(",")){
                    String[] part = smsBody.split(",");
                    try {
                        double lat = Double.parseDouble(part[1]);
                        double lng = Double.parseDouble(part[0]);
                        database.addLocation(new Location(phoneNumber,lat,lng));
                        setMotorPosition(lat,lng);
                    }catch (Exception e){

                    }

                }
            }

        }
    }

    private void setMotorPosition(double lat, double lng){
        LatLng location = new LatLng(lat,lng);
        float zoomLevel = 16.0f;
        googleMap.addMarker(new MarkerOptions().position(location).title("Motor Location"));
        googleMap.moveCamera(CameraUpdateFactory.newLatLngZoom(location,zoomLevel));
    }
}
