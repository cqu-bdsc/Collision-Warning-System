package com.github.cqu_bdsc.collision_warning_system.database;

import com.raizlabs.android.dbflow.annotation.Column;
import com.raizlabs.android.dbflow.annotation.PrimaryKey;
import com.raizlabs.android.dbflow.annotation.Table;
import com.raizlabs.android.dbflow.structure.BaseModel;

@Table(database = AppDataBase.class)
public class ResultModel extends BaseModel {

    @PrimaryKey(autoincrement = true)
    private int resultID;

    @Column
    private int id;

    @Column
    private int time;

    @Column
    private double distance;

    @Column
    private  boolean warning;

    @Column
    private int type ;

    @Column
    private long timeStamp1;

    @Column
    private long receiverTimeStamp;

    @Column
    private long sendTimeStamp;

    @Column
    private long timeStamp2;

    public int getResultID() {
        return resultID;
    }

    public void setResultID(int resultID) {
        this.resultID = resultID;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public long getTimeStamp1() {
        return timeStamp1;
    }

    public void setTimeStamp1(long timeStamp1) {
        this.timeStamp1 = timeStamp1;
    }

    public long getReceiverTimeStamp() {
        return receiverTimeStamp;
    }

    public void setReceiverTimeStamp(long receiverTimeStamp) {
        this.receiverTimeStamp = receiverTimeStamp;
    }

    public long getSendTimeStamp() {
        return sendTimeStamp;
    }

    public void setSendTimeStamp(long sendTimeStamp) {
        this.sendTimeStamp = sendTimeStamp;
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

    public void setTimeStamp2(long timeStamp2) {
        this.timeStamp2 = timeStamp2;
    }

    public long getTimeStamp2() {
        return timeStamp2;
    }
}
