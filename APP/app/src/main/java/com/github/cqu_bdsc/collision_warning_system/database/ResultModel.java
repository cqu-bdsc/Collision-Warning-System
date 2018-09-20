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
    private long receiverTimeStamp;

    @Column
    private long sendTimeStamp;

    @Column
    private long delay;

    public int getResultID() {
        return resultID;
    }

    public void setResultID(int resultID) {
        this.resultID = resultID;
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

    public long getDelay() {
        return delay;
    }

    public void setDelay(long delay) {
        this.delay = delay;
    }

}
