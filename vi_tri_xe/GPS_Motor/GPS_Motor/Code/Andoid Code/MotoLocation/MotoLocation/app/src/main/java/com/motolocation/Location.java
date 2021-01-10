package com.motolocation;

import java.io.Serializable;

public class Location implements Serializable {
    private int id;
    private String phoneNumber;
    private double lat;
    private double lng;

    public Location(){}

    public Location (String phoneNumber, double lat, double lng){
        this.phoneNumber = phoneNumber;
        this.lat = lat;
        this.lng = lng;
    }

    public Location (int id, String phoneNumber, double lat, double lng){
        this.id = id;
        this.phoneNumber = phoneNumber;
        this.lat = lat;
        this.lng = lng;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getPhoneNumber() {
        return phoneNumber;
    }

    public void setPhoneNumber(String phoneNumber) {
        this.phoneNumber = phoneNumber;
    }

    public double getLat() {
        return lat;
    }

    public void setLat(double lat) {
        this.lat = lat;
    }

    public double getLng() {
        return lng;
    }

    public void setLng(double lng) {
        this.lng = lng;
    }
}
