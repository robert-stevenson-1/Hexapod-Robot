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
        Vector3 diffVecTarget = targetLoc - HipPivotLocation;

        Debug.DrawLine(frTipLoc, targetLoc, Color.green, 600f);
        Debug.DrawLine(HipPivotLocation, targetLoc, Color.magenta, 600f);
        //Debug.DrawLine(HipPivotLocation, fr.hipLift.transform.position, Color.cyan, 600f);
        //Debug.DrawLine(fr.hipLift.transform.position, targetLoc, Color.blue, 600f);

        double newHipAngle;
        double newLiftAngle;
        double newKneeAngle;

        //calculate the leg length 
        double len1 = Math.Sqrt(Math.Pow(diffVecTarget.x, 2) + Math.Pow(diffVecTarget.z , 2));
        double len1Real = Vector3.Distance(HipPivotLocation, targetLoc);
        double len3 = len1 - fr.coxaOffset; 
        double len2 = Math.Sqrt(Math.Pow(len3, 2) + Math.Pow(diffVecTarget.y, 2));
        double len2Real = Vector3.Distance(fr.hipLift.transform.position, targetLoc);

        //calculate assistive values for IK when getting the hip rotation angles
        Vector3 diffVecStart = frTipLoc - HipPivotLocation;
        double alpha4 = Math.Atan2(diffVecStart.z, diffVecStart.x) * 180 / Math.PI;
        double alpha5 = 180 - (alpha4 + fr.rotationAngle);

        double alpha6 = Math.Atan2(diffVecTarget.z, diffVecTarget.x) * 180 / Math.PI; //value in degrees


        //calculate the hip rotation angle
        //  same as: 90 - Atan(Xd/Zd)
        newHipAngle = 180 - (alpha6 + alpha5);

        //calculate assistive values for IK when getting values for knee and lift angles
      

        //double alpha1 = Math.Atan2(len3, diffVecTarget.y) * 180 / Math.PI; //value in degrees
        double alpha1 = Math.Acos(Math.Abs(diffVecTarget.y)/len2) * 180 / Math.PI; //value in degrees

        double alpha2 = Math.Acos(
            (Math.Pow(fr.tibiaLength, 2) - Math.Pow(fr.femurLength, 2) - Math.Pow(len2, 2)) /
            (-2 * fr.femurLength * len2)
        ) * 180 / Math.PI; //value in degrees

        double alpha3 = Math.Acos(
            (Math.Pow(len2, 2) - (Math.Pow(fr.femurLength, 2)) - Math.Pow(fr.tibiaLength, 2)) / 
                (-2 * fr.femurLength * fr.tibiaLength)
            ) * 180 / Math.PI;//value in degrees

        newLiftAngle = alpha1 + alpha2;

        newKneeAngle = 180 - alpha3;

        
        fr.moveLeg(newHipAngle, newLiftAngle, newKneeAngle);
        
        Debug.Log("New Hip Rotation angle: " + newHipAngle +
                  "\nNew Hip Lift angle: " + newLiftAngle +
                  "\nNew Knee bend angle: " + newKneeAngle +
                  "\nalpha1: " + alpha1 +
                  "\nalpha2: " + alpha2 +
                  "\nalpha3: " + alpha3 +
                  "\nCurrent Vector Loc: " + frTipLoc.ToString("F5") +
                  "\nDifference Vector: " + diffVecTarget.ToString("F5") +
                  "\nTarget Vector Loc: " + targetLoc.ToString("F5") +
                  "\nLen 1: " + len1 +
                  "\nLen 2 (smart): " + Vector3.Distance(fr.hipLift.transform.position, targetLoc) +
                  "\nLen 2: " + len2);
        
        Debug.DrawLine(HipPivotLocation, fr.legTip.position, Color.cyan, 600f);

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
