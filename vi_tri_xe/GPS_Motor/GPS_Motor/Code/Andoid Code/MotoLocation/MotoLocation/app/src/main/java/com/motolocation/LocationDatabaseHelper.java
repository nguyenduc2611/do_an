package com.motolocation;

import android.app.ListActivity;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import java.util.ArrayList;
import java.util.List;

public class LocationDatabaseHelper extends SQLiteOpenHelper {
    private static final String TAG = LocationDatabaseHelper.class.getName();
    private static final int DB_VERSION = 1;
    private static final String DB_NAME = "Location_Manager";
    private static final String TABLE_LOCATION = "location";
    private static final String COLUMN_ID = "id";
    private static final String COLUMN_PHONENUMBER = "phone_number";
    private static final String COLUMN_LAT = "lat";
    private static final String COLIMN_LNG = "lng";

    public LocationDatabaseHelper(Context context){
        super(context,DB_NAME,null,DB_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        String createScript = "CREATE TABLE " + TABLE_LOCATION + "("
                + COLUMN_ID + " INTEGER PRIMARY KEY,"
                + COLUMN_PHONENUMBER + " TEXT,"
                + COLUMN_LAT + " TEXT,"
                + COLIMN_LNG + " TEXT" + ")";
        db.execSQL(createScript);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_LOCATION);
        onCreate(db);
    }


    public void addLocation(Location location){
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put(COLUMN_PHONENUMBER,location.getPhoneNumber());
        values.put(COLUMN_LAT, location.getLat());
        values.put(COLIMN_LNG, location.getLng());

        db.insert(TABLE_LOCATION, null,values);
        db.close();
    }


    public List<Location> getLocations (String phoneNumber){
        List<Location> listLocation = new ArrayList<>();
        SQLiteDatabase db = this.getReadableDatabase();

        String selectQuery = "SELECT * FROM " + TABLE_LOCATION + " WHERE phone_number='" + phoneNumber +"'";
        Cursor cursor = db.rawQuery(selectQuery,null);
        if (cursor != null) {
            if (cursor.moveToFirst()) {
                do {
                    String phone = cursor.getString(cursor.getColumnIndex(COLUMN_PHONENUMBER));
                    Double lat = cursor.getDouble(cursor.getColumnIndex(COLUMN_LAT));
                    Double lng = cursor.getDouble(cursor.getColumnIndex(COLIMN_LNG));
                    listLocation.add(new Location(phone,lat,lng));
                } while (cursor.moveToNext());
            }
            cursor.close();
        }
        return listLocation;
    }
}
