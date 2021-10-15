using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
 
public class RobotController : MonoBehaviour
{
    public Robot robot;
    public SerialPort arduinoStream;
    public float nextTime;
    public string data = "";
    public string lastData = "";
    public float sampleRate = 0f;

    // Start is called before the first frame update
    void Start()
    {
        //setup the arduino com port
        arduinoStream = new SerialPort("COM4", 9600);
        arduinoStream.ReadTimeout = 50;
        arduinoStream.Open(); //open the serial com port

        nextTime = Time.time;

        robot.fr.moveLeg(RobotConfig.FR_ROTATE_INIT_ANGLE, RobotConfig.FR_LIFT_INIT_ANGLE, RobotConfig.FR_KNEE_INIT_ANGLE);
        robot.mr.moveLeg(RobotConfig.MR_ROTATE_INIT_ANGLE, RobotConfig.MR_LIFT_INIT_ANGLE, RobotConfig.MR_KNEE_INIT_ANGLE);
        robot.br.moveLeg(RobotConfig.BR_ROTATE_INIT_ANGLE, RobotConfig.BR_LIFT_INIT_ANGLE, RobotConfig.BR_KNEE_INIT_ANGLE);
        robot.fl.moveLeg(RobotConfig.FL_ROTATE_INIT_ANGLE, RobotConfig.FL_LIFT_INIT_ANGLE, RobotConfig.FL_KNEE_INIT_ANGLE);
        robot.ml.moveLeg(RobotConfig.ML_ROTATE_INIT_ANGLE, RobotConfig.ML_LIFT_INIT_ANGLE, RobotConfig.ML_KNEE_INIT_ANGLE);
        robot.bl.moveLeg(RobotConfig.BL_ROTATE_INIT_ANGLE, RobotConfig.BL_LIFT_INIT_ANGLE, RobotConfig.BL_KNEE_INIT_ANGLE);
    }

    // Update is called once per frame
    void Update()
    {

        if(Time.time > nextTime)
        {
            if (arduinoStream.IsOpen)
            {
                data = readRobot(50);
            }
            else
            {
                print("Port Not Open");
            }
            nextTime = Time.time+sampleRate;
        }

        if (data != null && data != lastData)
        {
            print("Data: " + data);
            lastData = data;
        }
    }

    string readRobot(int timeout = 0)
    {
        arduinoStream.ReadTimeout = timeout;
        try
        {
            return arduinoStream.ReadLine();
        }
        catch(System.TimeoutException e)
        {
            return null;
        }
    }
}
