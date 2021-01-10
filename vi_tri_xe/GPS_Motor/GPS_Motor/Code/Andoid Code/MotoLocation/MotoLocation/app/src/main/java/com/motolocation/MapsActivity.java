package com.motolocation;

import android.Manifest;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.RequiresApi;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Toast;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;

import java.io.IOException;
import java.net.InetAddress;

public class MapsActivity extends AppCompatActivity implements OnMapReadyCallback {

    private GoogleMap   mMap;
    private EditText    phoneNumberEt;
    private Button checkPositionBt;
    private SMSNewBroadcastReceiver receiver;
    private String phoneNumberStr;
    private SharedPreferences preferences;
    private SharedPreferences.Editor editor;

    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().setTitle(R.string.actionbar_title);
        setContentView(R.layout.activity_maps);
        requestPermissions();

        phoneNumberEt    = findViewById(R.id.et_phone_number);
        phoneNumberEt.setFocusable(false);
        checkPositionBt  = findViewById(R.id.bt_check);
        preferences = getSharedPreferences(getString(R.string.prefsPhone),MODE_PRIVATE);
        String defaultPhone = preferences.getString(getString(R.string.prefs_phone_value),"");
        phoneNumberEt.setText(defaultPhone);

        if(!isOnline()){
            Toast.makeText(this,"Không có internet", Toast.LENGTH_SHORT).show();
        }

        // Obtain the SupportMapFragment and get notified when the map is ready to be used.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);

        phoneNumberEt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                phoneNumberEt.setFocusableInTouchMode(true);
            }
        });

        checkPositionBt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                phoneNumberStr = phoneNumberEt.getText().toString();
                if(!phoneNumberStr.equalsIgnoreCase("")){
                    phoneNumberEt.setFocusable(false);
                    editor = preferences.edit();
                    editor.clear();
                    editor.putString(getString(R.string.prefs_phone_value),phoneNumberStr);
                    editor.commit();
                    receiver = new SMSNewBroadcastReceiver(MapsActivity.this,phoneNumberStr,mMap);
                    IntentFilter filter = new IntentFilter("android.provider.Telephony.SMS_RECEIVED");
                    registerReceiver(receiver, filter);
                    Log.d("SMS","register receiver");
                }else
                    Toast.makeText(MapsActivity.this,"Số điện thoại không được để trống",Toast.LENGTH_SHORT).show();
            }
        });

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(receiver != null)
            unregisterReceiver(receiver);
    }

    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;
        mMap.setMapType(GoogleMap.MAP_TYPE_NORMAL);
        mMap.getUiSettings().setZoomControlsEnabled(true);
        mMap.getUiSettings().setZoomGesturesEnabled(true);
        mMap.getUiSettings().setCompassEnabled(true);
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(new LatLng(20.980881,105.784120),12.0f));
    }

    final String[] permissions = new String[]{Manifest.permission.RECEIVE_SMS, Manifest.permission.READ_SMS, Manifest.permission.ACCESS_FINE_LOCATION};
    @RequiresApi(api = Build.VERSION_CODES.M)
    private void requestPermissions(){
        ActivityCompat.requestPermissions(MapsActivity.this,permissions,1);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {

    }

    public boolean isOnline() {
        Runtime runtime = Runtime.getRuntime();
        try {
            Process ipProcess = runtime.exec("/system/bin/ping -c 1 8.8.8.8");
            int     exitValue = ipProcess.waitFor();
            return (exitValue == 0);
        }
        catch (IOException e)          { e.printStackTrace(); }
        catch (InterruptedException e) { e.printStackTrace(); }

        return false;
    }
}
