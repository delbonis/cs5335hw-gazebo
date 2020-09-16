
#include <iostream>
#include <math.h>

#include "robot.hh"

using std::cout;
using std::endl;

const double goal_x = 20.0;
const double goal_y = 0.0;
bool done = false;

void callback(Robot* robot) {

    cout << endl;
    cout << "robot x = " << robot->pos_x << endl;
    cout << "robot y = " << robot->pos_y << endl;
    cout << "robot t = " << robot->pos_t << endl;

    double dx = goal_x - robot->pos_x;
    double dy = goal_y - robot->pos_y;
    double target_dist = sqrt(dx * dx + dy * dy);
    double target_angle = atan2(dy, dx);
    double target_ang_diff = target_angle - robot->pos_t;

    if (abs(dx) < 0.75 && abs(dy) < 0.75) {
        cout << "we win!" << endl;
        robot->set_vel(0.0);
        robot->set_turn(0.0);
        robot->done();
        return;
    }

    cout << "target angle = " << target_angle << endl;
    cout << "target ang diff = " << target_ang_diff << endl;

    bool is_blocker = false;
    float jiggle = 0.0;
    int jiggles = 0;
    for (LaserHit hit : robot->hits) {
        if (hit.range < 5.0) {
            float ht_diff = abs(hit.angle - target_angle);

            //cout << "hit @ t = " << hit.angle << " range = " << hit.range << endl;

            if (ht_diff < 0.5) {
                is_blocker = true;

                double jig_mag = fmin(0.02 / hit.range, 0.05);
                if (hit.angle < 0) {
                    jiggle -= jig_mag;
                } else {
                    jiggle += jig_mag;
                }
                jiggles++;
            }
        }
    }

    cout << "net jiggle = " << jiggle << " (" << jiggles << " points)" << endl;

    // clamp to [-0.2, 0.2] to avoid overturning and getting stuck
    jiggle = fmin(fmax(jiggle, -0.2), 0.2);

    if (!is_blocker) {
        cout << "nothing in the way" << endl;

        // Do some math to figure out how to turn.
        double ang_mag = abs(target_ang_diff);
        double turn_mag = ang_mag > 0.5 ? 0.1 : 0.02;
        if (ang_mag > 0.01) {
            robot->set_turn(target_ang_diff < 0 ? turn_mag : -turn_mag);
        } else {
            cout << "on course" << endl;
            robot->set_turn(0.0);
        }

        robot->set_vel(10.0);
    } else {

        cout << "avoiding collision" << endl;

        robot->set_turn(jiggle);
        robot->set_vel(4.0);

    }

    //if (turn) {
    //    robot->set_vel(6.0);
    //    robot->set_turn(0.5);
    //} else {
    //    robot->set_vel(10.0);
    //    robot->set_turn(-0.02);
    //}
}

int main(int argc, char* argv[]) {
    cout << "making robot" << endl;
    Robot robot(argc, argv, callback);
    robot.do_stuff();

    return 0;
}
