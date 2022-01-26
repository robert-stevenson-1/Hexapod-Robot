using System;
using System.Collections;
using UnityEngine;
using System.IO.Ports;
using System.Threading;


public class RobotController : MonoBehaviour
{
    [Header("Robot Assignment")]
    public Robot robot;

    [Header("Robot Communication")] 
    public String comPort = "COM4";
    public int baudRate = 9600;
    public int readTimeout = 10000;
    public int writeTimeout = 200;
    public int ThreadDelay = 200;
    public SerialPort arduinoStream;

    [Space(5)]
    //Communication thread
    public Thread ComThread;
    //[Space(5)]
    //public float nextTime;
    //public float sampleRate;

    [Space(5)]
    public string data = "";
    public string lastData = "";

    [Space(5)]
    public bool allowReceive = true;
    public bool allowTransmit = true;
    public bool canReceive = true;
    public bool canTransmit = true;

    [Header("Debug")]
    public bool startUpDone;
    public float upTime;
    public bool lockLocation = false;


    // Start is called before the first frame update
    void Start()
    {
        try
        {
            //setup the arduino com port
            arduinoStream = new SerialPort(comPort, baudRate);
            arduinoStream.ReadTimeout = readTimeout;
            arduinoStream.WriteTimeout = writeTimeout;
            arduinoStream.ReadBufferSize = 8192;
            arduinoStream.WriteBufferSize = 256;
            arduinoStream.Parity = Parity.None;
            arduinoStream.StopBits = StopBits.One;
            arduinoStream.DataBits = 8;
            arduinoStream.DtrEnable = true;
            arduinoStream.Handshake = Handshake.None;
            arduinoStream.Open(); //open the serial com port

            //if the port is open then start the communication to the robot on a separate thread
            ComThread = new Thread(talkToRobot);
            ComThread.Start();

            }
        catch (Exception e)
        {
            Debug.LogError(e);
        }


        robot.fr.moveLeg(RobotConfig.FR_ROTATE_INIT_ANGLE, RobotConfig.FR_LIFT_INIT_ANGLE,
            RobotConfig.FR_KNEE_INIT_ANGLE);
        robot.mr.moveLeg(RobotConfig.MR_ROTATE_INIT_ANGLE, RobotConfig.MR_LIFT_INIT_ANGLE,
            RobotConfig.MR_KNEE_INIT_ANGLE);
        robot.br.moveLeg(RobotConfig.BR_ROTATE_INIT_ANGLE, RobotConfig.BR_LIFT_INIT_ANGLE,
            RobotConfig.BR_KNEE_INIT_ANGLE);
        robot.fl.moveLeg(RobotConfig.FL_ROTATE_INIT_ANGLE, RobotConfig.FL_LIFT_INIT_ANGLE,
            RobotConfig.FL_KNEE_INIT_ANGLE);
        robot.ml.moveLeg(RobotConfig.ML_ROTATE_INIT_ANGLE, RobotConfig.ML_LIFT_INIT_ANGLE,
            RobotConfig.ML_KNEE_INIT_ANGLE);
        robot.bl.moveLeg(RobotConfig.BL_ROTATE_INIT_ANGLE, RobotConfig.BL_LIFT_INIT_ANGLE,
            RobotConfig.BL_KNEE_INIT_ANGLE);


        //counter initial startup launch force from intersecting parts on start up
        startUpDone = true;
        Debug.Log(getRobotData(robot));

    }

    // Update is called once per frame
    void Update()
    {

    }

    private void LateUpdate()
    {
        upTime += Time.fixedDeltaTime;
        //counter inital startup launch force from intersecting parts on start up
        if (startUpDone && upTime > 1f && !lockLocation)
        {
            //Unity simulation on only requirement
            robot.robotBody.immovable = false;
            robot.robotBody.velocity = new Vector3(0, 0, 0);
            startUpDone = false;
        }
    }

    private void OnApplicationQuit()
    {
    arduinoStream.Close();
    ComThread.Abort();
    }

    public void talkToRobot() //Runs on a seperate thread
    {
        while (true)
        {
            Debug.Log("Comms Thread Running");
            if (arduinoStream.IsOpen)
            {
                //check to see if we are allowed to read data from the robot
                if (canReceive && allowReceive)
                {
                    //StartCoroutine(ReadRobot(200f));
                    data = ReadRobot(readTimeout);
                    canTransmit = true;
                    canReceive = false;
                }


                //check to see if we are allowed to send data to the robot
                if (canTransmit && allowTransmit)
                {
                    string toSend = getRobotData(robot);
                    if (toSend != data)
                    {
                        sendRobot(toSend, writeTimeout);
                    }
                    canTransmit = false;
                    canReceive = true;
                }
                Thread.Sleep(ThreadDelay);

            }
            else
            {
                Debug.LogWarning("Port Not Open");
                Debug.LogWarning("Killing ComThread...");
                ComThread.Abort();
                Debug.LogWarning("Killed ComThread");

            }
            if (data != null && data != lastData || true)
            {
                //print("Data: " + data);
                lastData = data;
            }
        }
    }
    
    /*public IEnumerator ReadRobot(float timeout = 10000f)
    {
        DateTime initialTime = DateTime.Now;
        DateTime nowTime;
        TimeSpan diff = default(TimeSpan);
        string value;
        //arduinoStream.ReadTimeout = timeout;
        do
        {
            try
            {
                value = arduinoStream.ReadLine();
            }
            catch (TimeoutException e)
            {
                value = "";
                print(e);
            }

            if (value != "")
            {
                data = value;
                yield break;
            }

            nowTime = DateTime.Now;
            diff = nowTime - initialTime;
        } while (diff.Milliseconds < timeout);
    }*/
    string ReadRobot(int timeout = 1)
    {
        string value="";
        arduinoStream.ReadTimeout = timeout;
        try
        {
            arduinoStream.BaseStream.Flush();
            value = arduinoStream.ReadLine();
        }
        catch (TimeoutException e)
        {
            value = "";
            print(e);
        }

        if (value != "")
        {
            return value;
        }
        else
        {
            return "";
        }
    }

    void sendRobot(string data, int timeout = 1)
    {
        arduinoStream.WriteTimeout = timeout;
        try
        { 
            //Debug.Log("Sent: " + data);
            arduinoStream.WriteLine(data);
            arduinoStream.BaseStream.Flush();
        }
        catch (System.TimeoutException e)
        {
            print(e);
        }
    }

    string getRobotData(Robot robot)
    {
        string data = "";

        data += "<" + (int)robot.fr.rotationAngle;
        data += " " + (int)robot.fr.liftAngle;
        data += " " + (int)robot.fr.kneeAngle;
                  
        data += " " + (int)robot.mr.rotationAngle;
        data += " " + (int)robot.mr.liftAngle;
        data += " " + (int)robot.mr.kneeAngle;
                  
        data += " " + (int)robot.br.rotationAngle;
        data += " " + (int)robot.br.liftAngle;
        data += " " + (int)robot.br.kneeAngle;
                  
        data += " " + (int)robot.fr.rotationAngle;
        data += " " + (int)robot.fr.liftAngle;
        data += " " + (int)robot.fr.kneeAngle;
                  
        data += " " + (int)robot.mr.rotationAngle;
        data += " " + (int)robot.mr.liftAngle;
        data += " " + (int)robot.mr.kneeAngle;
                  
        data += " " + (int)robot.br.rotationAngle;
        data += " " + (int)robot.br.liftAngle;
        data += " " + (int)robot.br.kneeAngle + ">" ;

        return data;
    }

    public void disconnectCOM()
    {
        arduinoStream.Close();
    }
}