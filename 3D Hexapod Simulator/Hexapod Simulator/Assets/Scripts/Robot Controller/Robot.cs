using System;
using UnityEngine;

public class Robot : MonoBehaviour
{
    [Header("Robot Config")]
    public ArticulationBody robotBody;
    public Leg fr; //front right leg
    public Leg fl; //front left leg
    public Leg mr; //middle right leg
    public Leg ml; //middle left leg
    public Leg br; //back right leg
    public Leg bl; //back left leg

    public void MoveForward(float distance)
    {
        Vector3 frTipLoc = fr.legTip.position;
        Vector3 targetLoc = frTipLoc + new Vector3(0f, 0f, distance);
        Vector3 HipPivotLocation = fr.hipRotate.transform.position;
        Vector3 diffVec = targetLoc - HipPivotLocation;

        Debug.DrawLine(frTipLoc, targetLoc, Color.green, 600f);
        Debug.DrawLine(HipPivotLocation, targetLoc, Color.magenta, 600f);
        Debug.DrawLine(HipPivotLocation, fr.hipLift.transform.position, Color.cyan, 600f);
        Debug.DrawLine(fr.hipLift.transform.position, targetLoc, Color.blue, 600f);

        double newHipAngle = 0f;
        double newKneeAngle = 0f;
        double newLiftAngle = 0f;
        double legLengthTop = 0f;
        double legLengthSide = 0f;
        double baseToEffectorDist = 0;

        double l = Vector3.Distance(fr.hipLift.transform.position, targetLoc);
        double coxaOffset = Vector3.Distance(fr.hipRotate.transform.position, fr.hipLift.transform.position);

        //calculate the length of the leg with its foot at the new position (From the Lift Pivot of the hip)
        //baseToEffectorDist = Math.Sqrt(Math.Pow(diffVec.x, 2) + Math.Pow(diffVec.z, 2));

        //calculate the length of the leg with its foot at the new position (From the Lift Pivot of the hip)
        //  (Must account of the offset from the hip rotation pivot to the hip lift pivot) 
        legLengthTop = Math.Sqrt(Math.Pow(diffVec.x - fr.hipLift.transform.position.x, 2) + Math.Pow(diffVec.z - fr.hipLift.transform.position.z, 2));
        legLengthSide = Math.Sqrt(Math.Pow(legLengthTop, 2) + Math.Pow(diffVec.y, 2));
        //legLength = Math.Sqrt(Math.Pow(baseToEffectorDist - coxaOffset, 2) + Math.Pow(diffVec.z , 2));


        //GRABBING HIP LIFT POSITION PROBABLY BAD, CAUSING DRIFT?




        //calculate the hip rotation angle
        //  same as: 90 - Atan(Xd/Zd)
        newHipAngle = Math.Atan2(diffVec.z, diffVec.x) * 180 / Math.PI; //value in degrees

        //calculate assistive values for IK when getting values for knee and lift angles
        double alpha1 = Math.Acos(
            (Math.Pow(fr.tibiaLength, 2) - (Math.Pow(legLengthSide, 2)) - Math.Pow(fr.femurLength, 2)) / 
                (-2 * legLengthSide * fr.femurLength)
            ) * 180 / Math.PI;//value in degrees
        
        double alpha2 = Math.Acos(
            (Math.Pow(legLengthSide, 2) - Math.Pow(fr.femurLength, 2) - Math.Pow(fr.tibiaLength, 2)) /
                (-2 * fr.tibiaLength * fr.femurLength)
            ) * 180 / Math.PI; //value in degrees

        double alpha3 = Math.Acos(diffVec.y / legLengthSide) * 180 / Math.PI; //value in degrees

        newLiftAngle = (alpha1 + alpha3);

        newKneeAngle = 180 - alpha2;

        Debug.Log("New Hip Rotation angle: " + newHipAngle +
                  "\nNew Hip Lift angle: " + newLiftAngle +
                  "\nNew Knee bend angle: " + newKneeAngle +
                  "\nalpha1: " + alpha1 +
                  "\nalpha2: " + alpha2 +
                  "\nalpha3: " + alpha3 +
                  "\nCurrent Vector Loc: " + frTipLoc.ToString("F5") +
                  "\nDifference Vector: " + diffVec.ToString("F5") +
                  "\nTarget Vector Loc: " + targetLoc.ToString("F5") +
                  "\nLeg Length Top: " + legLengthTop +
                  "\nLeg Length Side: " + legLengthSide);

        fr.moveLeg((int)newHipAngle, (int)newLiftAngle, (int)newKneeAngle);
    }

    public void MoveBackwards(float distance)
    {
        MoveForward(-distance);
    }

    public void StrafeRight(float distance)
    {

    }
    public void StafeLeft(float distance)
    {
        StrafeRight(-distance);
    }

}
