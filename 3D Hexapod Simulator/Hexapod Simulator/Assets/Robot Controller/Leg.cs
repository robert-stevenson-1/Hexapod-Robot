using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Leg : MonoBehaviour
{
    public ArticulationBody hipRotate;
    public ArticulationBody hipLift;
    public ArticulationBody knee;

    public int rotationAngle;
    public int liftAngle;
    public int kneeAngle;

    public bool isLeft = false;

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
        hipRotate.yDrive = drive; //replace the legs drive in the hip used to rotate the leg
    }

    //Lift the leg by setting the Hip lift Angle
    public void liftLeg(int angle)
    {
        //fetch the drive for the legs hip
        ArticulationDrive drive = hipLift.zDrive;
        drive.target = angle; //update the drives target to the desired angle
        hipLift.zDrive = drive; //replace the legs drive in the hip used to lift the leg
    }
    
    //Bend the leg's knee by setting the knee's Angle
    public void bendKnee(int angle)
    {
        //fetch the drive for the knee
        ArticulationDrive drive = knee.zDrive;
        drive.target = angle; //update the drives target to the desired angle
        knee.zDrive = drive; //replace the legs drive in the knee used to bend the knee
    }

    private void Start()
    {
        rotationAngle = ((int)(hipRotate.yDrive).target);
        liftAngle = ((int)(hipLift.zDrive).target);
        kneeAngle = ((int)(hipRotate.zDrive).target);
    }
    private void Update()
    {
        
    }
}
