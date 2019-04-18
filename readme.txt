This is the source code of new Motor Board in 2019.
The main function of the code is:
1. The new Motor Board receives position data from TX1/TX2;
2. Send SyncWrite position packages to Dynamixel motors;
3. Send SyncRead packages to Dynamixel motors and foot;
4. Receive data from motor, foot and IMU, pack them up, send to TX1/TX2.