#include <AlfredoConnect.h>
#include <BluetoothSerial.h>
#include <Alfredo_NoU2.h>
NoU_Motor indexer_intake(1);
NoU_Motor pivot(2);
NoU_Motor shooter_bottom(3);
NoU_Motor shooter_top(4);
NoU_Motor rightMotor(5);
NoU_Motor leftMotor(6);
NoU_Servo s1(1);
NoU_Servo s2(2);
BluetoothSerial bluetooth;

void setup () { 
  bluetooth.begin("Raider");
  AlfredoConnect.begin(bluetooth);
  RSL::initialize();
  RSL::setState(RSL_ENABLED);
}

// Shoot uniform gives the same speed to the top and bottom rollers
void shootUniform(float speed){
  shooter_top.set(speed);
  shooter_bottom.set(speed);
}

// Shoot custom gives custom speed to the top and bottom rollers
void shootCustom(float speed1, float speed2){
  shooter_top.set(speed1);
  shooter_bottom.set(speed2);
}


void loop () { 

  bool ampSpeed = false;
  bool speakerSpeed = false;
  if (AlfredoConnect.getGamepadCount() >= 1) {
    float power = -AlfredoConnect.getAxis(0, 1);
    float turn = AlfredoConnect.getAxis(0, 2);

    // X for Intake
    if(AlfredoConnect.getButton(0,2)){
      indexer_intake.set(-1);
    }
    //B For Outtake
    else if(AlfredoConnect.getButton(0,1)){
      indexer_intake.set(1);
    }
    //LB Pivot Forward
    else if(AlfredoConnect.getButton(0,4)){
      pivot.set(-1);
    }
    //RB Pivot Backward
    else if(AlfredoConnect.getButton(0,5)){
      pivot.set(1);
    }
    //Left DPad for setting speaker speed
    else if(AlfredoConnect.getButton(0,14)){
      speakerSpeed = true;
      bluetooth.println("Speaker Speed Set");
    }
    //Right DPad for setting amp speed
    else if(AlfredoConnect.getButton(0,15)){
      ampSpeed = true;
      bluetooth.prinltn("Amp Speed Set")
    }
    else if(AlfredoConnect.getButton(0,13)){
      ampSpeed = false;
      speakerSpeed = false;
      bluetooth.println("Reset State");
    }
    //Y to Shoot
    else if(AlfredoConnect.getButton(0,3)){
      if(ampSpeed = true){
        shootCustom(-0.67,-0.7);
        delay(450);
        indexer_intake.set(-1);
        ampSpeed = false;
        speakerSpeed = false;
      }
      else if(speakerSpeed = true){
        shootUnifrom(-1);
        delay(450);
        indexer_intake.set(-1);
        speakerSpeed = false;
        ampSpeed = false;
      }
      else{
        bluetooth.println("QUEUE A SHOT FIRST!");
    }
    else{
      indexer_intake.set(0);
      pivot.set(0);
      shooter_bottom.set(0);
      shooter_top.set(0);
      rightMotor.set(0);
      leftMotor.set(0);
    }
    RSL::setState(RSL_ENABLED);
  } else {
    RSL::setState(RSL_DISABLED);
  }
  AlfredoConnect.update();
  RSL::update();
}