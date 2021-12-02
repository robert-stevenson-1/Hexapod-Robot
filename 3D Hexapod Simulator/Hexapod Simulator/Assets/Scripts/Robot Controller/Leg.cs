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
    public int rotationAngle;
    public int liftAngle;
    public int kneeAngle;

    [Header("Leg Parameters")]
    public float femurLength;
    public float tibiaLength;
    [Space(5)]
    public bool isLeft = false;


    private void Start()
    {
        rotationAngle = ((int)(hipRotate.yDrive).target);
        liftAngle = ((int)(hipLift.zDrive).target);
        kneeAngle = ((int)(hipRotate.zDrive).target);

        femurLength = Vector3.Distance(hipRotate.transform.position, knee.transform.position);
        tibiaLength = Vector3.Distance(knee.transform.position, legTip.transform.position);
    }
    private void Update()
    {

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

    public void moveLeg(int rotateAngle, int liftAngle, int kneeAngle)
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
    public void rotateLeg(int angle)
    {
        //fetch the drive for the legs hip rotation
        ArticulationDrive drive = hipRotate.yDrive;
        drive.target = angle; //update the drives target to the desired angle
        rotationAngle = angle;
        hipRotate.yDrive = drive; //replace the legs drive in the hip used to rotate the leg
    }

    //Lift the leg by setting the Hip lift Angle
    public void liftLeg(int angle)
    {
        //fetch the drive for the legs hip
        ArticulationDrive drive = hipLift.zDrive;
        drive.target = angle; //update the drives target to the desired angle
        liftAngle = angle;
        hipLift.zDrive = drive; //replace the legs drive in the hip used to lift the leg
    }
    
    //Bend the leg's knee by setting the knee's Angle
    public void bendKnee(int angle)
    {
        //fetch the drive for the knee
        ArticulationDrive drive = knee.zDrive;
        drive.target = angle; //update the drives target to the desired angle
        kneeAngle = angle;
        knee.zDrive = drive; //replace the legs drive in the knee used to bend the knee
    }

}
