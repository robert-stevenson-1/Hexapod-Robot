using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Leg : MonoBehaviour
{
    [Header("Leg Components")]
    public ArticulationBody hipRotate;
    public ArticulationBody hipLift;
    public ArticulationBody knee;
    public Transform legTip;

    [Header("Current Leg Joint angle")]
    public float rotationAngle;
    public float liftAngle;
    public float kneeAngle;

    [Header("Leg Parameters")]
    // 1.0f == 1 meter
    public float coxaOffset = 0.035f; //35mm
    public float femurLength = 0.90f; //90mm
    public float tibiaLength = 0.115f; //115mm
    [Space(5)]
    public bool isLeft = false;


    private void Start()
    {
        rotationAngle = ((int)(hipRotate.yDrive).target);
        liftAngle = ((int)(hipLift.zDrive).target);
        kneeAngle = ((int)(hipRotate.zDrive).target);

        //femurLength = Vector3.Distance(hipRotate.transform.position, knee.transform.position);
        //tibiaLength = Vector3.Distance(knee.transform.position, legTip.transform.position);
    }
    /*   Leg()
       {
           rotationAngle = ((int)(hipRotate.yDrive).target);
           liftAngle = ((int)(hipLift.zDrive).target);
           kneeAngle = ((int)(hipRotate.zDrive).target);
       }*/

    /*    Leg(ArticulationBody hipRotate, ArticulationBody hipLift, ArticulationBody knee, bool isLeft)
        {
            this.hipRotate = hipRotate;
            this.hipLift = hipLift;
            this.knee = knee;
            this.isLeft = isLeft;

            rotationAngle = ((int)(hipRotate.yDrive).target);
            liftAngle = ((int)(hipLift.zDrive).target);
            kneeAngle = ((int)(hipRotate.zDrive).target);
        }*/

    public void moveLeg(double rotateAngle, double liftAngle, double kneeAngle)
    {
        if (this.isLeft)
        {
            liftLeg(liftAngle);

            rotateLeg(rotateAngle);

            bendKnee(kneeAngle);

        }
        else
        {
            liftLeg(liftAngle);

            rotateLeg(rotateAngle);

            bendKnee(kneeAngle);
        }
    }

    //Rotate the leg by setting the Hip rotation Angle
    public void rotateLeg(double angle)
    {
        //check if we are in the correct bounds
        if (Mathf.Abs((float)angle) >= 0 && Mathf.Abs((float)angle) < 180)
        {
            //fetch the drive for the legs hip rotation
            ArticulationDrive drive = hipRotate.yDrive;
            drive.target = (float)angle; //update the drives target to the desired angle
            rotationAngle = (float)angle;
            hipRotate.yDrive = drive; //replace the legs drive in the hip used to rotate the leg   
        }
    }

    //Lift the leg by setting the Hip lift Angle
    public void liftLeg(double angle)
    {
        //check if we are in the correct bounds
        if (angle >= 0 && angle < 180)
        {
            //fetch the drive for the legs hip
            ArticulationDrive drive = hipLift.zDrive;
            drive.target = (float) angle; //update the drives target to the desired angle
            liftAngle = (float) angle;
            hipLift.zDrive = drive; //replace the legs drive in the hip used to lift the leg
        }
    }

    //Bend the leg's knee by setting the knee's Angle
    public void bendKnee(double angle)
    {
        //check if we are in the correct bounds
        if (angle >= 0 && angle < 180)
        {
            //fetch the drive for the knee
            ArticulationDrive drive = knee.zDrive;
            drive.target = (float) angle; //update the drives target to the desired angle
            kneeAngle = (float) angle;
            knee.zDrive = drive; //replace the legs drive in the knee used to bend the knee
        }
    }
}
