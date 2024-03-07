#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Round_Button.H>
#include <vector>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <random>

#define VELOCITY 0.01667f

/*Enum for Ball types*/
enum BALL_TYPE
{
    BEACHBALL,
    BASKETBALL,
    SOCCERBALL
};

/*The Flyweight class for keeping the intrinsic properties*/
class BallFlyweight
{
public:
    BallFlyweight(std::string imageFile, int radius) : radius(radius)
    {
        image = new Fl_PNG_Image(imageFile.c_str());
    }
    friend class Ball_Context;

private:
    int radius;
    Fl_PNG_Image *image;
};

/* Component class holding the context */
class Ball_Context : public Fl_Widget
{
public:
    float pos_x, pos_y; // Position
    float vel_x, vel_y; // Velocity

    Ball_Context(float _x, float _y,                                                                         //
                 float _vx, float _vy, float _radius,                                                        //
                 BallFlyweight *_state) :                                                                    //
                                          Fl_Widget(_x, _y, (2 * _state->radius), (2 * _state->radius), ""), //
                                          pos_x(_x), pos_y(_y), vel_x(_vx), vel_y(_vy), _intrinsicStates(_state)
    {
        /*constructor*/
    }

    // Updates Component position
    void updatePos(float delta)
    {
        pos_x += vel_x * delta;
        pos_y += vel_y * delta;

        // Check if the ball hits the window boundary
        if ((pos_x < 0) || //
            ((pos_x + (2 * _intrinsicStates->radius)) > parent()->w()))
        {
            vel_x = -vel_x; // Reverse x direction
        }
        if ((pos_y < (0 + parent()->h() * 0.1)) || //
            ((pos_y + (2 * _intrinsicStates->radius)) > parent()->h()))
        {
            vel_y = -vel_y; // Reverse y direction
        }
    }

    // Draw the component on the screen
    void draw()
    {
        fl_color(FL_BLUE);
        _intrinsicStates->image->draw(pos_x, pos_y,                   //
                                      (2 * _intrinsicStates->radius), //
                                      (2 * _intrinsicStates->radius));
    }

private:
    BallFlyweight *_intrinsicStates;
};

/* Manages components and simulates */
class BallSimulator : public Fl_Widget
{
public:
    BallSimulator(int x, int y, int w, int h) : Fl_Widget(x, y, w, h, ""), gen(rd()), dist(300, 150) { srand(time(nullptr)); }

    void setBallType(BALL_TYPE type)
    {
        current_type = type;
    }

    // Add a particle to the simulation
    Ball_Context *addParticle(float x, float y, float vx, float vy, float radius)
    {
        BallFlyweight *ballState;

        if (current_type == BEACHBALL)
            ballState = new BallFlyweight("ball_beach.png", 25);
        else if (current_type == BASKETBALL)
            ballState = new BallFlyweight("ball_basket.png", 25);
        else if (current_type == SOCCERBALL)
            ballState = new BallFlyweight("ball_soccer.png", 25);

        Ball_Context *ball = new Ball_Context(x, y, vx, vy, radius, ballState);

        particles.push_back(ball);
        return ball;
    }

    // Updates position of each particles created
    void updateAllParticlesPos(float delta)
    {
        for (auto &particle : particles)
        {
            particle->updatePos(delta);
        }
    }

    // handles mouse click event
    int handle(int event)
    {
        if (event == FL_PUSH)
        {
            int i = 5; // creates 5 balls at a time
            while (i--)
            {
                int sign = ((rand() % 2) == 0) ? 1 : -1;
                float vel_x = sign * dist(gen);
                float vel_y = sign * dist(gen);
                float radius = 25.0;
                float centerX = Fl::event_x();
                float centerY = Fl::event_y();

                Ball_Context *ball = addParticle(centerX, centerY, vel_x, vel_y, radius);

                parent()->add(ball);
            }
            return 1;
        }

        return Fl_Widget::handle(event);
    }

    // Draws each particle on the screen
    void draw()
    {
        for (auto &particle : particles)
        {
            particle->draw();
        }
    }

private:
    std::vector<Ball_Context *> particles;
    BALL_TYPE current_type = SOCCERBALL;
    std::random_device rd;
    std::mt19937 gen;
    std::normal_distribution<> dist;
};

/* creates radio button for Ball types choices */
class ChoicePanel : public Fl_Widget
{
public:
    ChoicePanel(int x, int y, int w, int h) : Fl_Widget(x, y, w, h),                                //
                                              panel(x, y, w, h),                                    //
                                              radio1(w * 0.3, h / 2, 100, 30, "Beach Ball"),        //
                                              radio2(w * 0.3 * 1.5, h / 2, 100, 30, "Basket Ball"), //
                                              radio3(w * 0.3 * 2, h / 2, 100, 30, "Soccer Ball"),   //
                                              val1(BEACHBALL), val2(BASKETBALL), val3(SOCCERBALL)
    {
        panel.box(FL_FLAT_BOX);
        panel.color(FL_GRAY);
        radio1.type(FL_RADIO_BUTTON);
        radio2.type(FL_RADIO_BUTTON);
        radio3.type(FL_RADIO_BUTTON);
        radio3.set();
        radio1.callback(radio_callback, &val1);
        radio2.callback(radio_callback, &val2);
        radio3.callback(radio_callback, &val3);
    }

    static void radio_callback(Fl_Widget *widget, void *data)
    {
        Fl_Round_Button *radio = (Fl_Round_Button *)widget;

        if (radio->value())
        {
            BALL_TYPE value = *(BALL_TYPE *)data;
            simulator->setBallType(value);
        }
    }

    void setSimulator(BallSimulator *sim)
    {
        this->simulator = sim;
    }

    int handle(int event) override
    {
        if (event == FL_PUSH)
        {
            return 1;
        }

        return Fl_Widget::handle(event);
    }
    void draw() override
    {
    }

private:
    Fl_Box panel;
    Fl_Round_Button radio1, radio2, radio3;
    BALL_TYPE val1, val2, val3;
    static BallSimulator *simulator;
};
BallSimulator *ChoicePanel::simulator = nullptr;

/* FLTK window for visualizing the simulation */
class SimulationWindow : public Fl_Window
{
public:
    SimulationWindow(int width, int height, const char *title) : Fl_Window(width, height, title), delta(VELOCITY), //
                                                                 panel(0, 0, width, height * 0.1),                 //
                                                                 simulation(0, height * 0.1, width, height * 0.9)
    {
        // Setup FLTK window
        color(FL_BLACK);
        end();
        panel.setSimulator(&simulation);
        Fl::add_timeout(delta, timer_callback, this);
    }

    // Timer callback for updating simulation
    static void timer_callback(void *userdata)
    {
        SimulationWindow *window = static_cast<SimulationWindow *>(userdata);

        window->simulation.updateAllParticlesPos(window->delta);

        window->redraw();

        Fl::repeat_timeout(window->delta, timer_callback, userdata);
    }

    // Draw callback for rendering simulation
    void draw() override
    {
        Fl_Window::draw();

        panel.draw();

        simulation.draw();
    }

private:
    ChoicePanel panel;
    BallSimulator simulation;
    float delta;
};

int main()
{
    SimulationWindow window(1280, 720, "Bouncing Balls Simulation");
    window.show();

    return Fl::run();
}