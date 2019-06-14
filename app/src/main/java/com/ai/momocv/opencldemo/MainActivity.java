package com.ai.momocv.opencldemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.lang.reflect.Field;

public class MainActivity extends AppCompatActivity {


    // Used to load the 'native-lib' library on application startup.
    static {
        try {
            setLibraryPath("/system/lib64");
        } catch (Exception e) {

            Log.d("MMCVOPENCL", "error:" + e.toString());
        }
        System.loadLibrary("native-lib");

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ClassLoader  classLoader = getClassLoader();
        Log.d("MMCVOPENCL", "classLoader:" + classLoader.toString());


        // (System.getProperty("java.library.path"): /system/lib64:/product/lib64 GOOGLEPIXEL3 ANDROID10 API28
       // (System.getProperty("java.library.path"): /oem/lib:/vendor/lib:/system/lib vivox9 android6.0.1 api23
        //                                          /oem/lib64:/vendor/lib64:/system/lib64
        Log.d("MMCVOPENCL", "(System.getProperty(\"java.library.path\"): "+(System.getProperty("java.library.path")) + "   (System.getProperty(\"os.arch \")):" + (System.getProperty("os.arch")) );


        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();



    /**
     * Sets the java library path to the specified path
     * @usage 动态更改sys_paths,使得usr_paths 重新初始化
     * @param path the new library path
     * @throws Exception
     */
    public static void setLibraryPath(String path) throws Exception {
        System.setProperty("java.library.path", path);

//        Field fieldSysPath = ClassLoader.class.getDeclaredField("usr_paths");
//        fieldSysPath.setAccessible(true);
//        fieldSysPath.set(null, null);

      /////  set sys_paths to null
        final Field sysPathsField = ClassLoader.class.getDeclaredField("path");
        sysPathsField.setAccessible(true);
        sysPathsField.set(null, null);
    }


}
