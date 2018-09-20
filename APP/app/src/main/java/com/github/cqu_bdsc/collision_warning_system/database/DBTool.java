package com.github.cqu_bdsc.collision_warning_system.database;

import android.app.IntentService;
import android.content.Intent;
import android.os.Environment;
import android.support.annotation.Nullable;
import android.util.Log;
import android.widget.Toast;

import com.github.cqu_bdsc.collision_warning_system.DAO.Message;
import com.github.cqu_bdsc.collision_warning_system.MainActivity;
import com.github.cqu_bdsc.collision_warning_system.udp.ReceiveThread;
import com.raizlabs.android.dbflow.sql.language.SQLite;
import com.raizlabs.android.dbflow.sql.language.Select;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.channels.FileChannel;
import java.sql.SQLTransactionRollbackException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

public class DBTool extends IntentService {

    /**
     * Creates an IntentService.  Invoked by your subclass's constructor.
     *
     * @param name Used to name the worker thread, important only for debugging.
     */
    public DBTool(String name) {
        super(name);
    }
    public DBTool(){
        super("DBTool");
    }
    public static final String TAG = "DBTool";
    public static final String DATABASE_PATH    = "/mnt/sdcard/Downloads/";
    public static final String DATABASE_FLIENAME = "databasebackup.db";

    public static final String ACTION_ADD       = "ACTION_ADD";
    public static final String ACTION_DELETE    = "ACTION_DELETE";
    public static final String ACTION_SELETE    = "ACTION_SELETE";
    public static final String ACTION_COPY      = "ACTION_COPY";

    public static final String TABLE_NAME               = "TABLE_NAME";
    public static final String TABLE_MESSAGE            = "TABLE_MESSAGE";
    public static final String TABLE_RESULT             = "TABLE_RESULT";
    public static final String TABLE_LOG                = "TABLE_LOG";
    public static final String TABLE_ALL                = "TABLE_ALL";

    /*MessageModel*/
    public static final String MESSAGE_ID   = "MESSAGE_ID";
    public static final String MESSAGE_TIMESTAMP = "MESSAGE_TIMESTAMP";
    public static final String MESSAGE_SPEED = "MESSAGE_SPEED";
    public static final String MESSAGE_DIRECTION = "MESSAGE_DIRECTION";
    public static final String MESSAGE_LAT = "MESSAGE_LAT";
    public static final String MESSAGE_LON = "MESSAGE_LON";
    public static final String MESSAGE_ACE = "MESSAGE_ACE";
    public static final String MESSAGE_MAC = "MESSAGE_MAC";

    /*ResultModel*/
    public static final String RESULT_ID = "RESULT_ID";
    public static final String RESULT_TIME = "RESULT_TIME";
    public static final String RESULT_DISTANCE = "RESULT_DISTANCE";
    public static final String RESULT_WARNING = "RESULT_WARNING";
    public static final String RESULT_RECEIVERTIMESTAMP = "RESULT_RECEIVERTIMESTAMP";
    public static final String RESULT_SENDTIMESTAMP = "RESULT_SENDTIMESTAMP";
    public static final String RESULT_DELAY = "RESULT_DELAY";

    /*LogModel*/
    public static final String LOG_TIMESTAMP = "LOG_TIMESTAMP";
    public static final String LOG_DATA      = "LOG_DATA";
    public static final String LOG_CONTEXT   = "LOG_CONTEXT";


    @Override
    protected void onHandleIntent(@Nullable Intent intent) {
        String action = intent.getAction();
        /**
         * 添加操作
         */
        if (action.equals(DBTool.ACTION_ADD)){
            String tableName = intent.getExtras().getString(DBTool.TABLE_NAME);
            if (tableName.equals(DBTool.TABLE_RESULT)){
                int id = intent.getExtras().getInt(RESULT_ID);
                int time = intent.getExtras().getInt(RESULT_TIME);
                double distance = intent.getExtras().getDouble(RESULT_DISTANCE);
                boolean warning = intent.getExtras().getBoolean(RESULT_WARNING);
                long receiverTimeStamp = intent.getExtras().getLong(RESULT_RECEIVERTIMESTAMP);
                long sendTimeStamp = intent.getExtras().getLong(RESULT_SENDTIMESTAMP);
                long delay = intent.getExtras().getLong(RESULT_DELAY);
                insertResult(id, time,  distance,  warning, receiverTimeStamp, sendTimeStamp, delay);
            }else if (tableName.equals(DBTool.TABLE_MESSAGE)){
                int id = intent.getExtras().getInt(DBTool.MESSAGE_ID);
                long timeStamp = intent.getExtras().getLong(DBTool.MESSAGE_TIMESTAMP);
                float speed = intent.getExtras().getFloat(DBTool.MESSAGE_SPEED);
                float direction = intent.getExtras().getFloat(DBTool.MESSAGE_DIRECTION);
                double lat = intent.getExtras().getDouble(DBTool.MESSAGE_LAT);
                double lon = intent.getExtras().getDouble(DBTool.MESSAGE_LON);
                double ace = intent.getExtras().getDouble(DBTool.MESSAGE_ACE);
                String mac = intent.getExtras().getString(DBTool.MESSAGE_MAC);
                insertMessage(id, timeStamp, speed,  direction,  lat, lon,  ace, mac);
            }else if (tableName.equals(DBTool.TABLE_LOG)){
                long timeStamp = intent.getExtras().getLong(DBTool.LOG_TIMESTAMP);
                String data = intent.getExtras().getString(DBTool.LOG_DATA);
                String context = intent.getExtras().getString(DBTool.LOG_CONTEXT);
                insertLog(timeStamp, data, context);
            } else {

            }
        }
        /**
         * 删除操作
         */
        else if (action.equals(DBTool.ACTION_DELETE)){
            String tableName = intent.getExtras().getString(DBTool.TABLE_NAME);
            if (tableName.equals(DBTool.TABLE_MESSAGE)){
                List<MessageModel> messageModels = SQLite.select().from(MessageModel.class).queryList();
                for (MessageModel messageModel:messageModels){
                    messageModel.delete();
                }
            } else if (tableName.equals(DBTool.TABLE_RESULT)){
                List<ResultModel> resultModels = SQLite.select().from(ResultModel.class).queryList();
                for (ResultModel resultModel:resultModels){
                    resultModel.delete();
                }
            } else {

            }
        }
        /**
         * 查询操作
         */
        else if (action.equals(DBTool.ACTION_SELETE)){
            String tableName = intent.getExtras().getString(DBTool.TABLE_NAME);
            if (tableName.equals(DBTool.TABLE_MESSAGE)){
                //返回所有查询结果
                List<MessageModel> messageModels = new Select().from(MessageModel.class).queryList();
                int number = messageModels.size();
                sendLog("Message表有"+String.valueOf(number)+"条记录");

            }else if (tableName.equals(DBTool.TABLE_RESULT)){
                List<ResultModel> resultModels = new Select().from(ResultModel.class).queryList();
                int number = resultModels.size();
                sendLog("Result表有"+String.valueOf(number)+"条记录");
            }else if (tableName.equals(DBTool.TABLE_ALL)){
                List<MessageModel> messageModels = new Select().from(MessageModel.class).queryList();
                int numberMessage = messageModels.size();
                List<ResultModel> resultModels = new Select().from(ResultModel.class).queryList();
                int numberResult = resultModels.size();
                List<LogModel> logModels = new Select().from(LogModel.class).queryList();
                int numberLog = logModels.size();
                sendSQL("Message表有"+String.valueOf(numberMessage)+"条记录  "+"Result表有"+String.valueOf(numberResult)+"条记录  "+"Log表中有"+ String.valueOf(numberLog)+"条记录");
            }
        }
        /***********************
         * 备份数据库
         */
        else if (action.equals(DBTool.ACTION_COPY)){
            try {
                SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd-HH:mm:ss");
                Date date = new Date(System.currentTimeMillis());
                String filePath = DATABASE_PATH+simpleDateFormat.format(date)+DATABASE_FLIENAME;
                createFile(filePath);
                File backupDB = new File(filePath);
                if (backupDB.canWrite()) {
                    String currentDBPath = "/data/data/" + getPackageName() + "/databases/"+ AppDataBase.NAME+".db";
                    File currentDB = new File(currentDBPath);

                    if (currentDB.exists()) {
                        FileChannel src = new FileInputStream(currentDB).getChannel();
                        FileChannel dst = new FileOutputStream(backupDB).getChannel();
                        dst.transferFrom(src, 0, src.size());
                        src.close();
                        dst.close();
                        sendSQL("备份数据库成功");
                    } else {
                        sendSQL("数据库不存在");
                    }
                } else {
                    sendSQL("备份数据库失败，文件不可写");
                }
            } catch (Exception e) {
                e.printStackTrace();
                sendSQL("备份数据库失败"+ e.getMessage());
            }
        } else {

        }

    }

    private void insertMessage(int id, long timeStamp, float speed, float direction, double lat, double lon, double ace, String mac){
        MessageModel messageModel = new MessageModel();
        messageModel.setId(id);
        messageModel.setTimeStamp(timeStamp);
        messageModel.setSpeed(speed);
        messageModel.setDirection(direction);
        messageModel.setLat(lat);
        messageModel.setLon(lon);
        messageModel.setAce(ace);
        messageModel.setMac(mac);
        messageModel.insert();
        sendLog("插入Message成功");
    }

    private void insertResult(int id, int time, double distance, boolean warning,  long receiverTimeStamp, long sendTimeStamp, long delay){
        ResultModel resultModel = new ResultModel();
        resultModel.setId(id);
        resultModel.setTime(time);
        resultModel.setDistance(distance);
        resultModel.setWarning(warning);
        resultModel.setReceiverTimeStamp(receiverTimeStamp);
        resultModel.setSendTimeStamp(sendTimeStamp);
        resultModel.setDelay(delay);
        resultModel.insert();
        sendLog("插入Result成功");
    }

    private void insertLog(long timeStamp, String data, String context){
        LogModel logModel = new LogModel();
        logModel.setTimeStamp(timeStamp);
        logModel.setData(data);
        logModel.setContext(context);
        logModel.insert();
        sendLog("插入Log成功");
    }

    public void sendLog(String log){
        Intent intent = new Intent();
        intent.setAction(ReceiveThread.ACTION_STRING);//告诉android将要执行什么功能
        intent.putExtra(ReceiveThread.STRING_CONTEXT, log);//信息
        getApplicationContext().sendBroadcast(intent);//广播信息
    }

    public void sendSQL(String log){
        Intent intent = new Intent();
        intent.setAction(MainActivity.ACTION_SQL);//告诉android将要执行什么功能
        intent.putExtra(ReceiveThread.STRING_CONTEXT, log);//信息
        getApplicationContext().sendBroadcast(intent);//广播信息
    }

    private void createFile(String pathname){
        File file = new File(pathname);
        if (!file.exists()){
            boolean isSuccess = false;
            try {
                isSuccess = file.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
                sendSQL("Create File failed."+e);
            }
            if (isSuccess){
                Log.d(TAG, "Create File Success");
                sendSQL("Create File failed.");
            }else {
                Log.d(TAG, "Create File Failed.");
                sendSQL("Create File Failed.");
            }
        }
    }

}
