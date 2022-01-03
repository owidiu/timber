#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
	VideoMode vm(1920, 1080); //video mode object

	RenderWindow window(vm, "Joculet", Style::Fullscreen); // create and open a window for the game

	Texture textureBackground; // create a texture to hold a graphic on the GPU

	textureBackground.loadFromFile("graphics/background.png");

	Sprite spriteBackground; // create sprite

	spriteBackground.setTexture(textureBackground); // attach texture to sprite

	spriteBackground.setPosition(0, 0);

	Texture textureTree; // tree sprite
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	Texture textureBee; // bee sprite
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	bool beeActive = false;
	float beeSpeed = 0.0f;

	Texture textureCloud; // cloud sprite
	textureCloud.loadFromFile("graphics/cloud.png");
	const int NUM_CLOUDS = 4;
	Sprite clouds[NUM_CLOUDS];
	int cloudSpeeds[NUM_CLOUDS];
	bool cloudsActive[NUM_CLOUDS];

	for (int i = 0; i < NUM_CLOUDS; i++) 
	{
		clouds[i].setTexture(textureCloud);
		clouds[i].setPosition(-300, i * 150);
		cloudsActive[i] = false;
		cloudSpeeds[i] = 0;

	}

	Clock clock; // time
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 40;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	bool paused = true; // track whether the game is running
	int score = 0;

	Text messageText;
	Text scoreText;
	Text fpsText;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	messageText.setFont(font);
	scoreText.setFont(font);
	fpsText.setFont(font);


	// set up the fps text
	fpsText.setFillColor(Color::White);
	fpsText.setCharacterSize(100);
	fpsText.setPosition(1200, 20);

	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);
	RectangleShape rect1;
	rect1.setFillColor(sf::Color(0, 0, 0, 150));
	rect1.setSize(Vector2f(600, 105));
	rect1.setPosition(0, 30);
	RectangleShape rect2;
	rect2.setFillColor(sf::Color(0, 0, 0, 150));
	rect2.setSize(Vector2f(1000, 105));
	rect2.setPosition(1150, 30);

	Texture textureBranch; // prepare 5 branches
	textureBranch.loadFromFile("graphics/branch.png");

	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	// player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	side playerSide = side::LEFT; // player starts left

	// game over
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false; // player input

	// sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	// the score
	int lastDrawn = 0;

	while (window.isOpen())
	{
		// score ++;
		Event event;
		while (window.pollEvent(event))
		{


			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;

				// hide the axe
				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}

		}

		// exit game
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// start game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			// reset time and score
			score = 0;
			timeRemaining = 6;

			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			spriteRIP.setPosition(2000, 2000);
			spritePlayer.setPosition(580, 720);
			acceptInput = true;


		}

		if (acceptInput)
		{
			
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score=score+30;
				timeRemaining += (2 / score) + .10;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);

				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
				chop.play();

			}
			
			// Handle the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;
				score=score+30;
				timeRemaining += (2 / score) + .10;

				spriteAxe.setPosition(AXE_POSITION_LEFT,spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);

				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				chop.play();

			}
			

		}

		if (!paused)
		{
			Time dt = clock.restart();
			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond *timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f) {
				paused = true;
				messageText.setString("No time left");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +textRect.width / 2.0f,textRect.top +textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				outOfTime.play();
			}

			if (!beeActive)
			{
				srand((int)time(0) * 30);
				beeSpeed = (rand() % 200) + 200;
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				spriteBee.setPosition(spriteBee.getPosition().x -(beeSpeed * dt.asSeconds()),spriteBee.getPosition().y);
				if (spriteBee.getPosition().x < -100)	beeActive = false;
			}

			for (int i = 0; i < NUM_CLOUDS; i++)
			{
				if (!cloudsActive[i])
				{
					srand((int)time(0) * i);
					cloudSpeeds[i] = (rand() % 200);
					srand((int)time(0) * i);
					float height = (rand() % 150);
					clouds[i].setPosition(-200, height);
					cloudsActive[i] = true;
				}
				else
				{
					clouds[i].setPosition(clouds[i].getPosition().x +(cloudSpeeds[i] * dt.asSeconds()),clouds[i].getPosition().y);
					if (clouds[i].getPosition().x > 1920)	cloudsActive[i] = false;
				}
			}
			
			//	score and the frame rate
			lastDrawn++;
			if (lastDrawn == 120) {
				std::stringstream ss;
				ss << "Score = " << score;
				scoreText.setString(ss.str());

				std::stringstream ss2;
				ss2 << "FPS = " << 1 / dt.asSeconds();
				fpsText.setString(ss2.str());
				lastDrawn = 0;
			}

			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					branches[i].setPosition(610, height);
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(0);
				}
				else
				{
					branches[i].setPosition(3000, height);
				}
			}
				
			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				if (spriteLog.getPosition().x < -100 ||spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// branch position on player position
			if (branchPositions[5] == playerSide)
			{
				// death
				paused = true;
				acceptInput = false;

				// game over
				spriteRIP.setPosition(0, 0);

				// hide	player
				spritePlayer.setPosition(2000, 660);

				//	hide axe
				spriteAxe.setPosition(2000, 660);

				//	hide bee
				spriteBee.setPosition(2000, 660);

				//	death sound
				death.play();


			}


		}

		 // clear everything
		window.clear();

		// new game
		window.draw(spriteBackground);

		// again ... again
		// clouds		
		for (int i = 0; i < NUM_CLOUDS; i++)
		{
			window.draw(clouds[i]);
		}

		// branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		// tree
		window.draw(spriteTree);

		// player
		window.draw(spritePlayer);

		// axe
		window.draw(spriteAxe);

		// log
		window.draw(spriteLog);

		// gameover
		window.draw(spriteRIP);

		// bee
		window.draw(spriteBee);

		// score
		window.draw(scoreText);

		// fps
		window.draw(fpsText);

		// timebar
		window.draw(timeBar);

		if (paused)
		{
			window.draw(messageText);
		}
		window.display();
	}
	return 0;
}

void updateBranches(int seed)
{
	// move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}
	srand((int)time(0) + seed); // new branch at position 0
	int r = (rand() % 5);

	switch (r) 
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[0] = side::RIGHT;
		break;

	default:
		branchPositions[0] = side::NONE;
		break;
	}


}