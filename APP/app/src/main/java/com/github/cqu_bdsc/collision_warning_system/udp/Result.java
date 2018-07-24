package com.github.cqu_bdsc.collision_warning_system.udp;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.Serializable;

public class Result implements Serializable {
    private int id;
    private int time;
    private double distance;
    private  boolean warning;

    Result(){
        id = 0;
        time = 0;
        distance = 0;
        warning = false;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getTime() {
        return time;
    }

    public void setTime(int time) {
        this.time = time;
    }

    public double getDistance() {
        return distance;
    }

    public void setDistance(double distance) {
        this.distance = distance;
    }

    public boolean isWarning() {
        return warning;
    }

    public void setWarning(boolean warning) {
        this.warning = warning;
    }

    public void fromJSON(JSONObject jsonObject){
        try {
            this.setId(jsonObject.getInt("id"));
            this.setTime(jsonObject.getInt("time"));
            this.setDistance(jsonObject.getDouble("distance"));
            this.setWarning(jsonObject.getBoolean("warning"));
        } catch (JSONException e) {
            e.printStackTrace();
            return;
        }

    }
}
