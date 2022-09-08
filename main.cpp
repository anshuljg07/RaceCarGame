// Author: Anshul Gowda
// Date Created: 10/20/21
//
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>

using namespace sf;
using namespace std;

const int num = 8; //checkpoints
int points[num][2] = {300, 610,
                      1270, 430,
                      1380, 2380,
                      1900, 2460,
                      1970, 1700,
                      2550, 1680,
                      2400, 3150,
                      500, 3300};

class Car{
public:
    /*Initialize the member variable to zero*/
    Car(){
        m_speed = 0;
        m_angle = 0;
        m_current_checkpoint = 0;
        m_background_x = 2850;
        m_background_y = 3630;
    }
    /*move() takes no arguments and increments/decrements the x member var and y member var based on its speed and angle*/
    void move(){
        //don't allow the car the leave the boarder
        float dx = m_background_x - m_x; //distance between the car's center and the x border
        float dy = m_background_y - m_y; //distance between the car's center and the y border
        if(dx <= 0){ //push car back
            m_x -= 15;
        }
        if(dy <= 0){ //push car back
            m_y -= 15;
        }
        if( dx >= m_background_x){ //push car forward
            m_x += 15;
        }
        if(dy >= m_background_y){ //push car forward
            m_y += 15;
        }

        //change the cars x and y position using the speed and its angle
        m_x += sin(m_angle) * m_speed;
        m_y -= cos(m_angle) * m_speed;
    }
    /*findTarget() takes no arguments and determines the angle member var (direction) the car will turn based on the closest checkpoint listed
     * in the points array*/
    void findTarget(){
        float tx = points[m_current_checkpoint][0];
        float ty = points[m_current_checkpoint][1];
        float beta = m_angle - atan2(tx - m_x, -ty + m_y); //beta is equal to the difference in angle between the cars current angle and the angle to the next checkpoint
        if (sin(beta) < 0) { //if beta is between pi and 2pi (180 and 360 degrres)
            m_angle += 0.005 * m_speed; //turn right
        }
        else{
            m_angle -= 0.005 * m_speed; //otherwise turn left
        }
        if ((m_x - tx) * (m_x - tx) + (m_y - ty) * (m_y - ty) < 25 * 25) {
            m_current_checkpoint = (m_current_checkpoint + 1) % num;
        }
    }
    //getter/setters for all the member vars
    float get_m_x() const{return  m_x;};
    void set_m_x(float x) {
        m_x = x;
    };
    float get_m_y() const{return m_y;};
    void set_m_y(float y){
        m_y = y;

    };
    float get_m_speed() const{return m_speed;};
    void set_m_speed(float speed) {m_speed = speed;};
    float get_m_angle() const{return m_angle;};
    void set_m_angle(float angle) {m_angle = angle;};
    float get_m_current_checkpoint() const{return m_current_checkpoint;};
    void set_m_current_checkpoint(float n) { m_current_checkpoint = n;};

    //used for the user's car and returns the next checkpoint, while also assigning to m_current_checkpoint member var
    int find_user_checkpoint(){
        float tx = points[m_current_checkpoint][0];
        float ty = points[m_current_checkpoint][1];
        if ((m_x - tx) * (m_x - tx) + (m_y - ty) * (m_y - ty) < 100 * 100) {
            m_current_checkpoint = (m_current_checkpoint + 1) % num;
        }
        return m_current_checkpoint;
    }
    //setter/getter function for m_background_x
    float get_m_background_x() const { return m_background_x;}
    void set_m_background_x(float mBackgroundX) {
        if (mBackgroundX > 0) {
            m_background_x = mBackgroundX;
        }
    }
    //setter/getter for m_background_y
    float get_m_background_y() const {return m_background_y;}
    void set_m_background_Y(float mBackgroundY) {
        if(mBackgroundY > 0) {
            m_background_y = mBackgroundY;
        }
    }

private:
    float m_x;
    float m_y, m_speed, m_angle;
    int m_current_checkpoint;
    float m_background_x, m_background_y;

};

int play_game();

int main() {
    play_game(); //call game loop
}
// takes no input, but simply contains the game loop, so that the game can be replayed recursively
int play_game(){
    int counter = 0;
    auto width = VideoMode::getDesktopMode().width; //get width and height of user's computer screen
    auto height = VideoMode::getDesktopMode().height;
    RenderWindow app(VideoMode(width, height), "Car Racing Game!");
    app.setFramerateLimit(60);

    Font timeFont;
    //advanced-pixel-lcd-7 by styel-7 license: free for personal use
    if(!timeFont.loadFromFile("fonts/advanced-pixel-lcd-7/advanced_pixel_lcd-7.ttf")){
        return EXIT_FAILURE;
    }

    //create choose vehicle page
    app.clear(Color::Black);
    Text Welcome;
    Welcome.setFont(timeFont);
    Texture fullheli, fullman, fullrocket, fullCar, t2, ManWalk;
    bool isHeli = false, isMan = false;

    //load fullsized versions of the vehicles for the selection page
    //blackheli by pngfind license: free to use with credit
    if(!fullheli.loadFromFile("images/heli_full_size.png")){
        return EXIT_FAILURE;
    }
    //illustration of man walking - aerial view by dushi82 license: free to use
    if(!fullman.loadFromFile("images/man_full_size.png")){
        return EXIT_FAILURE;
    }
    //spacecraft cartoon drawing by pngwing license: free to use
    if(!fullrocket.loadFromFile("images/rocket_full_size.png")){
        return EXIT_FAILURE;
    }
    if(!fullCar.loadFromFile("images/car.png")) { //load the race car using the address argument as a texture
        return EXIT_FAILURE;
    }
    fullheli.setSmooth(true);
    fullman.setSmooth(true);
    fullrocket.setSmooth(true);

    Sprite SHeli(fullheli), SMan(fullman), SRocket(fullrocket), SCarSmall(fullCar); //make them sprites
    Event welcome;
    SHeli.setOrigin(233.5, 309); //choose size, center, and position of vehicles for the selection page
    SMan.setOrigin(123, 203.5);
    SRocket.setOrigin(297.5, 240.5);
    SCarSmall.setOrigin(21.5, 22.5);
    SCarSmall.setColor(Color::Red);
    SHeli.scale(1.05, 1.05);
    SMan.scale(1.25, 1.25);
    SRocket.scale(1.25, 1.25);
    SCarSmall.scale(7,7);
    SHeli.setPosition(750, 400);
    SRocket.setPosition(2250, 400);
    SMan.setPosition(750, 1200);
    SCarSmall.setPosition(2250, 1200);

    //create rectangles and initialize characteristics to produce rectangles on selection page
    Text WelcomeT;
    WelcomeT.setFont(timeFont);
    WelcomeT.setString("Choose a vehicle\n\n\nto continue!!!"); //selection page text
    WelcomeT.setPosition(1150, 720);
    WelcomeT.setFillColor(Color::White);
    WelcomeT.setCharacterSize(40);
    WelcomeT.setOutlineColor(Color::Yellow);
    WelcomeT.setOutlineThickness(1);
    RectangleShape WelcomeTBox(Vector2f(800, 400));
    WelcomeTBox.setOrigin(400, 200);
    WelcomeTBox.setFillColor(Color::Transparent);
    WelcomeTBox.setOutlineThickness(40.f);
    WelcomeTBox.setOutlineColor(Color::Yellow);
    WelcomeTBox.setPosition(1500, 800);
    RectangleShape TopWB(Vector2f(40, 600));
    TopWB.setFillColor(Color::Yellow);
    TopWB.setOrigin(20, 300);
    TopWB.setPosition(1500, 300);
    RectangleShape BottomWB(TopWB);
    BottomWB.setPosition(1500, 1300);
    RectangleShape LeftWB(Vector2f(1100, 40));
    LeftWB.setFillColor(Color::Yellow);
    LeftWB.setOrigin(550, 20);
    LeftWB.setPosition(550, 800);
    app.pollEvent(welcome);
    RectangleShape RightWB(LeftWB);
    RightWB.setPosition(2450, 800);
    //Wait loop, that waits for a user to click a vehicle with their mouse
    do {
        Start:
        //draw vehicles, rectangles, and text for selection page
        app.clear(Color::Black);
        app.draw(WelcomeT);
        app.draw(SHeli);
        app.draw(SMan);
        app.draw(SRocket);
        app.draw(SCarSmall);
        app.draw(WelcomeTBox);
        app.draw(TopWB);
        app.draw(BottomWB);
        app.draw(LeftWB);
        app.draw(RightWB);
        app.display();
        if (Mouse::isButtonPressed(Mouse::Left)){ //if the user left clicks the mouse
            Vector2i MousePosition = Mouse::getPosition(app);
            if((MousePosition.x > 0 && MousePosition.x <= 1480)  && (MousePosition.y > 0 && MousePosition.y <= 780)){ //top left (heli)
                //blackheli by pngfind license: free to use with credit
                if(!t2.loadFromFile("images/heli.png")) {
                    return EXIT_FAILURE;
                }
                isHeli = true; //for future use to animate the heli rotors
                goto END;
            }
            else if((MousePosition.x >= 1520 && MousePosition.x < 3000)  && (MousePosition.y > 0 && MousePosition.y <= 780)){ //top right (spaceship)
                //spacecraft cartoon drawing by pngwing license: free to use
                if(!t2.loadFromFile("images/spaceship.png")) {
                    return EXIT_FAILURE;
                }
                goto END;
            }
            else if((MousePosition.x > 0 && MousePosition.x <= 1480)  && (MousePosition.y >= 820 && MousePosition.y <= 1600)){ //bottom left (man)
                //illustration of man walking - aerial view by dushi82 license: free to use
                if(!t2.loadFromFile("images/man.png")) {
                    return EXIT_FAILURE;
                }
                ManWalk.loadFromFile("images/man_walk.png");
                isMan = true;
                goto END;
            }
            else if((MousePosition.x >= 1520 && MousePosition.x < 3000)  && (MousePosition.y >= 820 && MousePosition.y <= 1600)){  //bottom right (car)
                if(!t2.loadFromFile("images/car.png")) {
                    return EXIT_FAILURE;
                }
                goto END;
            }
            else {
                goto Start;
            }
        }
    }
    while(welcome.type != Event::MouseButtonPressed); //wait for mouse input
    END:

    //load images for the racing game
    Texture t1, t3, checkpoint;
    int numframes = 0; //keep track of number of frames for animations
    bool switched[2] = {false, false}; //keep track of state of vehicles for animations
    if(!t1.loadFromFile("images/background.png")) { //load the background image (racetrack) as a texture
        return EXIT_FAILURE;
    }

    //play looped music
    sf::Music music;
    //Moose by Benjamin Tissot bensound.com license: free to use as long as a credit is provided.
    if(!music.openFromFile("sounds/bensound-moose.ogg")){
        return EXIT_FAILURE;
    }
    music.setLoop(true);
    music.play();
    //make the pictures smooth to make them look better
    t1.setSmooth(true);
    t2.setSmooth(true);

    //create sprites for the car and the background
    Sprite sBackground(t1), sCar(t2);
    sBackground.scale(2, 2); //scale the sprite by factor 2 for x and y
    sCar.scale(2, 2);

    sCar.setOrigin(21, 30);
    float R = 22;

    const int N = 5; //number of cars
    Car car[N]; //create 5 Car objects that will be the race cars in the game
    for (int i = 0; i < N; i++) {
        car[i].set_m_x(300 + i * 50); //initialize x position, so that for each car there is space between in the x direction
        car[i].set_m_y(1700 + i * 80); //initialize y position of each car, so that each car has vertical distance between them
        car[i].set_m_speed(7 + i); //initialize speed of each car
    }

    //define the max speed, acceleration, deacceleration, and the turn speed
    float speed = 0, angle = 0;
    float maxSpeed = 12.0;
    float acc = 0.2, dec = 0.3;
    float turnSpeed = 0.08;

    int offsetX = 0, offsetY = 0;

    int user_checkpoint = 0; //initialize the user car's checkpoint monitor to zero

    //create circle checkpoints and initialize everything but position
    CircleShape circleshape(30);
    circleshape.setOrigin(5, 5);
    circleshape.setFillColor(Color::Cyan);
    circleshape.setOutlineColor(Color::White);
    circleshape.setOutlineThickness(15.f);

    //create upper left corner timer
    auto start = chrono::system_clock::now();
    chrono::duration<double> elapsed;
    Text display_time;
    display_time.setFont(timeFont);
    display_time.setCharacterSize(20);
    display_time.setFillColor(Color::White);
    display_time.setStyle(Text::Bold);
    display_time.setPosition(10, 30);

    while (app.isOpen()) {
        if( numframes % 9 == 0){ //every nine frames switch texture to flipped version for animation
            if(isMan){
                if(switched[1] == false){
                    sCar.setTexture(ManWalk);
                    switched[1] = true;
                }
                else{
                    sCar.setTexture(t2);
                    switched[1] = false;
                }
            }
        }

        Event e;
        while (app.pollEvent(e)) { //if there is a pending event
            if (e.type == Event::Closed) {//if the pending event is to close the window, then close the window
                app.close();
            }
        }
        //create the countdown after the selection page (3,2, 1, 0)
        if(numframes == 0){
            chrono::seconds interval(1); //define a one second interval for countdown
            string three_two_one = "3210";
            Text Start_text;
            Start_text.setOrigin(65, 65);
            Start_text.setPosition(width/2, height/2);
            Start_text.setFont(timeFont);
            Start_text.setCharacterSize(130);
            Start_text.setOutlineThickness(20);
            Start_text.setFillColor(Color::Black);
            Start_text.setOutlineColor(Color::White);
            for (int i = 0;  i < three_two_one.size(); i++){
                app.clear(Color::Black);
                Start_text.setString(three_two_one[i]);
                app.draw(Start_text);
                app.display();
                this_thread::sleep_for(interval); //sleep an interval defined for 1 second, then display the next value
            }
        }


        //define key movements, and if a Up, Right, Down, or Left keyboard event occurs, load the associated variable.
        bool Up = false, Right = false, Down = false, Left = false;
        if (Keyboard::isKeyPressed(Keyboard::Up)){ //if the Up key is pressed then set the Up variable to true
            Up = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) { //if the Right key is pressed set the Right variable to true
            Right = true;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)){ //if the Down key is pressed set the Right variable to true
            Down = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)){ //if the Left key is pressed set the Left variable to true
            Left = true;
        }

        //car movement
        if (Up && speed < maxSpeed) { //if the up arrow is pressed and the car's speed is less than the max speed
            if (speed < 0){ // if the car has a negative speed deacclerate the car by the dec value
                speed += dec;
            }
            else { //if the car has a non-negative speed, then accelerate the car by adding the acceleratino value
                speed += acc;
            }
        }

        if (Down && speed > -maxSpeed) { //if the down arrow is pressed and the car is greater than it's max backwards speed
            if (speed > 0) { //if the car has a positive speed (moving forward), then deacclerate the car by the dec value
                speed -= dec;
            }
            else { //if the car has a negative speed (moving backwards), then deacclerate the car by the acc value
                speed -= acc;
            }
        }

        if (!Up && !Down) { //if the up or down arrows are not pressed
            if (speed - dec > 0){ //if the speed of the car is greater than the deacceleration value then deaccelerate the car by subtracting the dec value from speed
                speed -= dec;
            }
            else if (speed + dec < 0) { //if the speed of the car is less than the negative deacceleration value then deacclerate the car by adding the dec value to the negative speed
                speed += dec;
            }
            else { //if the speed is in the range(-dec, dec) then simply stop the car
                speed = 0;
            }
        }

        if (Right && speed != 0) { //if the right arrow is pressed and the car is moving
            angle += turnSpeed * speed / maxSpeed; //add to angle (turn right) by the ratio of the speed to the max speed times the turns speed (i.e. the faster you are going the faster you turn)
        }
        if (Left && speed != 0) { //if the left arrow is pressed and the car is moving
            angle -= turnSpeed * speed / maxSpeed; //subtract from angle (turn left) by ratio of the speed to max speed
        }

        //set the user's car's calculated speed and angle
        car[0].set_m_speed(speed);
        car[0].set_m_angle(angle);

        for (int i = 0; i < N; i++) {
            car[i].move(); //call the move() Car member function for each car in the game
        }
        for (int i = 1; i < N; i++) {
            car[i].findTarget(); //call the findTarget() member function for each car that is not the user's car
        }

        //collision
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int dx = 0, dy = 0;
                bool b = true;
                while (dx * dx + dy * dy < 4 * R * R && b) {
                    car[i].set_m_x(car[i].get_m_x() + dx / 10.0); //adjust car i's x position away from cary j's in the + direction
                    car[i].set_m_y(car[i].get_m_y() + dy / 10.0); //move car i's y position away from car j in the +  direction
                    car[j].set_m_x(car[j].get_m_x() - dx / 10.0); //move car j's x position away from car i in the negative direction
                    car[j].set_m_y(car[j].get_m_y() + dy / 10.0); //move car j's y position away from car i in the negative direction
                    dx = car[i].get_m_x() - car[j].get_m_x(); //calculate the distance between car i and j in the x direction
                    dy = car[i].get_m_y() - car[j].get_m_y(); //calculate the distance between car i and j in the y direction
                    if (!dx && !dy){ //if the distance between the car in the x direction or the y direction are zero exit the while loop
                        b = false;
                    }
                }
            }
        }


        app.clear(Color::Black); //clear the window for the new drawable objects to be displayed
        //keeps the car centered in the windows frame in the x direction and the y direction
        if (car[0].get_m_x() > 320) {
            offsetX = car[0].get_m_x() - 950;
        }
        if (car[0].get_m_y() > 240) {
            offsetY = car[0].get_m_y() - 500;
        }

        sBackground.setPosition(-offsetX, -offsetY); //set the position of the window to center the car in the middle of the window
        app.draw(sBackground);

        Color colors[10] = {Color::White, Color::Green, Color::Magenta, Color::Blue, Color::Red}; //contains the color objects used in the program

        for (int i = 0; i < N; i++) {
            sCar.setPosition(car[i].get_m_x() - offsetX, car[i].get_m_y()- offsetY); //initialize the position of each "car" sprite
            sCar.setRotation(car[i].get_m_angle() * 180 / 3.141593); //initialize the rotation of each "car" sprite
            sCar.setColor(colors[i]); //initialize the color of each car based on the color defined in the color vector of SFML colors
            app.draw(sCar); //draw the drawable "Car sprite
        }

        if(user_checkpoint != car[0].find_user_checkpoint()){//if the user car's checkpoint has changed (user passed a checkpoint)
            if(user_checkpoint == 0 && counter != 0){ // if the user has completed a lap and reached the first checkpoint again
                string end_time = "Elpased Time: " + to_string(elapsed.count() - 4) + " seconds\n\nPress R   to play again... \n\nPress N  to quit!";
                Text Endtime; //display the end time
                Endtime.setPosition(200, 300);
                Endtime.setFont(timeFont);
                Endtime.setFillColor(Color::Black);
                Endtime.setString(end_time);
                Endtime.setCharacterSize(60);
                Endtime.setOrigin(30, 30);
                Event playagain;
                app.pollEvent(playagain);

                do { //enter a wait loop after the game has finished to replay or exit the game
                    app.pollEvent(playagain);
                    app.clear(Color::White);
                    app.draw(Endtime);
                    if(Keyboard::isKeyPressed(Keyboard::N)){ //exit out of the game
                        music.stop();
                        app.close();
                        return 0;
                    }
                    app.display();
                }
                while(!(Keyboard::isKeyPressed(Keyboard::R)));
                if(Keyboard::isKeyPressed(Keyboard::R)){ //if user input chooses to play again, recursively call playgame(), while closing previous game
                    music.stop();
                    app.close();
                    play_game();
                }
            }
            counter++; //used to determine whether user car passed the first checkpoint more than once
            circleshape.setPosition(-offsetX + points[car[0].find_user_checkpoint()][0], -offsetY + points[car[0].find_user_checkpoint()][1]); // set position of the checkpoints at next checkpoint
            app.draw(circleshape);

        }
        else{
            circleshape.setPosition(-offsetX + points[user_checkpoint][0], -offsetY + points[user_checkpoint][1]); //display checkpoint
            app.draw(circleshape);
        }
        user_checkpoint = car[0].find_user_checkpoint(); //used to determine if the user_checkoint has changed

        auto end = chrono::system_clock::now(); //estimates the time of each frame, and is used for the timer
        elapsed = end - start; //contains the total elapsed time
        display_time.setString(to_string(elapsed.count() - 4));
        app.draw(display_time);

        app.display(); //display all drawable objects to the window
        numframes++;
    }
    return 0;
}
