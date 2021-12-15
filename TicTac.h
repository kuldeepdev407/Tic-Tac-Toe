#include "TGUI/TGUI.hpp"
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>

class TicTac
{
private:
	sf::RenderWindow* window;
	tgui::GuiSFML gui;
	sf::Texture backgroundTexture;
	sf::Sprite background;
	bool init;
	enum GameState {
		MENU,
		GAME,
		PAUSE,
		ABOUTUS,
		RESULT,
	};
	GameState gameState;
	sf::Sound sound,p1Sound,p2Sound,errSound,backSound;
	sf::SoundBuffer HoverSound,p1SoundBuff,p2SoundBuff,errSoundBuff,backSoundBuff;
	int gamearr[3][3]= { {0, 0, 0}, {0, 0, 0} ,{0, 0, 0} };//game array 0- no fill 1-x 2-o
	tgui::ButtonRenderer buttonStyle; 
	//when program is closeing
	void Close()
	{
		window->close();
		gui.removeAllWidgets();
		backSound.stop();
	}
public:
	void Start()
	{	
		init = true;
		gameState = GameState::MENU;
		sf::RenderWindow Window(sf::VideoMode(600, 400), "Tic Tac Toe", sf::Style::Close | sf::Style::Titlebar);
		sf::RectangleShape yellowBack;
		sf::RectangleShape v1(sf::Vector2f(15,220)), v2(sf::Vector2f(15, 220)), h1(sf::Vector2f(220, 15)), h2(sf::Vector2f(220, 15));
		sf::Vector2f shapepostion(196,88);//position of the # shape
		
		sf::Sprite cross;
		sf::Texture crossTexture;
		sf::Rect<float> clickablePlace;
		sf::CircleShape oshape;
		
		int winner = 0;
		bool chance=true; //0 - player one x - 1

		window = &Window;
		window->setVerticalSyncEnabled(true);
		gui.setTarget(*window);
		
		//Loading and adding backgournd texture
		if (!backgroundTexture.loadFromFile("Background.png") )
			std::cout<<"Error : Unable to load background"<<std::endl;
		background.setTexture(backgroundTexture);
		background.setPosition(0, 0);

		//setting up the sound
		HoverSound.loadFromFile("hover.wav");
		p1SoundBuff.loadFromFile("player1sound.wav");
		p2SoundBuff.loadFromFile("player2sound.wav");
		errSoundBuff.loadFromFile("error.wav");
		backSoundBuff.loadFromFile("backmusic.wav");
		sound.setBuffer(HoverSound);
		p1Sound.setBuffer(p1SoundBuff);
		p2Sound.setBuffer(p2SoundBuff);
		errSound.setBuffer(errSoundBuff);
		backSound.setBuffer(backSoundBuff);
		backSound.setLoop(true);

		//Setting up the Yellow backgorun
		yellowBack.setPosition(112, 51);
		yellowBack.setSize(sf::Vector2f(376, 279));
		yellowBack.setFillColor(sf::Color(254, 255, 181));

		//Setting up the Hash shape of the game
		v1.setPosition(shapepostion.x+70, shapepostion.y);
		v2.setPosition(shapepostion.x+70*2, shapepostion.y);
		h1.setPosition(shapepostion.x, shapepostion.y+70);
		h2.setPosition(shapepostion.x, shapepostion.y+70*2);
		v1.setFillColor(sf::Color(46, 46, 46, 255));
		v2.setFillColor(sf::Color(46, 46, 46, 255));
		h1.setFillColor(sf::Color(46, 46, 46, 255));
		h2.setFillColor(sf::Color(46, 46, 46, 255));

		//Adding up the crossshape
		if (!crossTexture.loadFromFile("crossshape.png"))
			std::cout << "Error : Unable to load crossshape" << std::endl;
		else
			cross.setTexture(crossTexture);	
		//setting up o shape player
		oshape.setRadius(10);
		oshape.setOutlineThickness(10);
		oshape.setOutlineColor(sf::Color(128, 27, 227));
		oshape.setFillColor(sf::Color::Transparent);

		//setting up clickable size
		clickablePlace.height = 70;
		clickablePlace.width = 70;
		backSound.play();
		while (window->isOpen())
		{
			//Intialiation on page change
			if (init)
			{
				init = false;
				switch (gameState)
				{
				case MENU: {
					tgui::Button::Ptr play = tgui::Button::create("Play"), 
						aboutus = tgui::Button::create("About us"), 
						exit = tgui::Button::create("Exit");
					tgui::Label::Ptr heading = tgui::Label::create("TIC TAC TOE");

					//setting up heading
					heading->setTextSize(48);
					heading->setPosition(155, 40);
					heading->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
					heading->getRenderer()->setTextColor(sf::Color::Black);
					gui.add(heading, "tictactoe");
					
					tgui::Outline outline;
					//setting up play button
					play->setSize(198, 54);
					play->setPosition(212, 117);
					play->setTextSize(24);
					play->getRenderer()->setTextColor(sf::Color::Black);
					play->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
					play->getRenderer()->setBorders(outline);
					//play->getRenderer()->setBorderColor(sf::Color::Transparent);
					play->getRenderer()->setBorderColorHover(sf::Color::Transparent);
					play->getRenderer()->setBorderColorDown(sf::Color::Transparent);
					play->getRenderer()->setBackgroundColor(sf::Color(239, 100, 100));
					play->getRenderer()->setBackgroundColorHover(sf::Color(200, 100, 100));
					play->getRenderer()->setBackgroundColorDown(sf::Color(200, 100, 100));
					play->onMouseEnter([&]() {
						sound.play();
					});
					play->onMouseRelease([&]() {
						init = true;
						gameState = GAME;
						gui.removeAllWidgets();
						});
					buttonStyle = *play->getRenderer();
					gui.add(play, "play");

					//setting up aboutus button
					aboutus->setSize(198, 54);
					aboutus->setPosition(212, 183);
					aboutus->setTextSize(24);
					aboutus->setRenderer(buttonStyle.clone());
					aboutus->onMouseEnter([&]() {
						sound.play();
						});
					aboutus->onMouseRelease([&]() {
						init = true;
						gameState = ABOUTUS;
						gui.removeAllWidgets();
						});
					gui.add(aboutus, "aboutus");

					//setting up aboutus button
					exit->setSize(198, 54);
					exit->setPosition(212, 248);
					exit->setTextSize(24);
					exit->onMouseEnter([&]() {
						sound.play();
						});
					exit->onMouseRelease([&]() {
						Close(); });
					exit->setRenderer(buttonStyle.clone());
					gui.add(exit, "exit");
				}break;
				
				case GAME: {
					tgui::Label::Ptr chancePlayer=tgui::Label::create("Player 1 Chance");
					tgui::Button::Ptr reset = tgui::Button::create("Reset");
					//setting up the setting of chance
					chancePlayer->setPosition(218, 23);
					chancePlayer->setTextSize(24);
					gui.add(chancePlayer, "playerChanceText");
					//setting up retry button
					reset->setTextSize(22);
					reset->setSize(100, 50);
					reset->setPosition(100, 350);
					reset->setRenderer(buttonStyle.clone());
					reset->onMouseRelease([&]() {
						init = true;
						gameState = GAME;
						gui.removeAllWidgets();
						});
					gui.add(reset);
					chance = true;
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 3; j++)
							gamearr[i][j] = 0;
					}
				}break;
				case ABOUTUS: {
					tgui::Label::Ptr heading=tgui::Label::create("ABOUT US");
					tgui::Button::Ptr back = tgui::Button::create("Back");
					tgui::Label::Ptr aboutme = tgui::Label::create("hi there my name is kuldeep. \n this is a project work of mine");
					
					//setting up heading
					heading->setPosition(180, 50);
					heading->setTextSize(44);
					heading->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
					gui.add(heading);

					//setting up about me
					aboutme->setPosition(120, 130);
					aboutme->setTextSize(24);
					aboutme->getRenderer()->setTextStyle(tgui::TextStyle::Italic);
					gui.add(aboutme);

					//setting up back button
					back->setPosition(240, 220);
					back->setTextSize(30);
					back->setRenderer(buttonStyle.clone());
					back->onMouseEnter([&]() {
						sound.play(); });
					back->onMouseRelease([&]() {
						init = true;
						gameState = MENU;
						gui.removeAllWidgets();
					});
					gui.add(back);
				}
					break;
				case PAUSE:
					break;
				case RESULT:
					tgui::Panel::Ptr transparentBack = tgui::Panel::create();
					tgui::Button::Ptr retry = tgui::Button::create("Retry");
					tgui::Button::Ptr exit = tgui::Button::create("Exit");
					tgui::Label::Ptr winnerText = tgui::Label::create();
					
					//adding pannel
					transparentBack->getRenderer()->setBackgroundColor({ 0, 0, 0, 175 });
					gui.add(transparentBack,"TransparentBack");
					
					//Settig up winner heading
					winnerText->setText(winner == 1 ? "Winner is Player 1" : winner==2?"Winner is Player 2":"   MATCH IS DRAW  ");
					winnerText->setTextSize(44);
					winnerText->setPosition(90, 100);
					winnerText->getRenderer()->setTextColor(sf::Color::White);
					winnerText->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
					gui.add(winnerText,"winnerText");

					//setting up the retry button
					retry->setRenderer(buttonStyle.clone());
					retry->setTextSize(44);
					retry->setPosition(120, 200);
					retry->onMouseRelease([&]() {
						gameState = GAME;
						init = true;
						gui.removeAllWidgets();
					});
					gui.add(retry,"Retry");
					
					//setting up the exit button
					exit->setRenderer(buttonStyle.clone());
					exit->setTextSize(44);
					exit->setPosition(330, 200);
					exit->onMouseRelease([&]() {
						init = true;
						gameState = MENU;
						gui.removeAllWidgets();
					});
					gui.add(exit,"ExitButton");
					
					break;
				}
			}

			//Events
			sf::Event events;
			while (window->pollEvent(events))
			{
				gui.handleEvent(events);
				switch (events.type)
				{
				case sf::Event::Closed:Close();
					break;
				default:
					break;
				}
				
				//event hadling accorind to state of game
				if (!init) {
					switch (gameState)
					{
					case MENU: {

					}break;
					case GAME:{
						
						if (events.type == sf::Event::MouseButtonReleased) {
							bool nozero = false;
							for (int i = 0; i < 3; i++) {
								for (int j = 0; j < 3; j++)
								{
									if (gamearr[i][j] == 0)
										nozero = true;
									clickablePlace.left = shapepostion.x + 80 * i;
									clickablePlace.top = shapepostion.y + 80 * j;
									if (clickablePlace.intersects(sf::FloatRect(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y, 1, 1)) && gamearr[i][j] == 0)
									{

										tgui::Label::Ptr playerChance = gui.get<tgui::Label>("playerChanceText");
										const char* chanceString;
										if (chance) {
											p1Sound.play();
											gamearr[i][j] = 1;
											playerChance->setText("Player 2 Chance");
										}
										else {
											p2Sound.play();
											gamearr[i][j] = 2;
											playerChance->setText("Player 1 Chance");
										}

										chance = !chance;
									}
									if (clickablePlace.intersects(sf::FloatRect(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y, 1, 1)) && gamearr[i][j] != 0) {
										errSound.play();
									}
								}
							}
							if (gamearr[0][0] == 1 && gamearr[0][1] == 1 && gamearr[0][2] == 1 ||
								gamearr[1][0] == 1 && gamearr[1][1] == 1 && gamearr[1][2] == 1 ||
								gamearr[2][0] == 1 && gamearr[2][1] == 1 && gamearr[2][2] == 1 ||
								gamearr[0][0] == 1 && gamearr[1][1] == 1 && gamearr[2][2] == 1 ||
								gamearr[0][0] == 1 && gamearr[1][0] == 1 && gamearr[2][0] == 1 ||
								gamearr[0][1] == 1 && gamearr[1][1] == 1 && gamearr[2][1] == 1 ||
								gamearr[0][2] == 1 && gamearr[1][2] == 1 && gamearr[2][2] == 1 ||
								gamearr[2][0] == 1 && gamearr[1][1] == 1 && gamearr[0][2] == 1)
							{
								winner = 1;
								gameState = RESULT;
								init = true;
							}
							else if (gamearr[0][0] == 2 && gamearr[0][1] == 2 && gamearr[0][2] == 2 ||
								gamearr[1][0] == 2 && gamearr[1][1] == 2 && gamearr[1][2] == 2 ||
								gamearr[2][0] == 2 && gamearr[2][1] == 2 && gamearr[2][2] == 2 ||
								gamearr[0][0] == 2 && gamearr[1][1] == 2 && gamearr[2][2] == 2 ||
								gamearr[0][0] == 2 && gamearr[1][0] == 2 && gamearr[2][0] == 2 ||
								gamearr[0][1] == 2 && gamearr[1][1] == 2 && gamearr[2][1] == 2 ||
								gamearr[0][2] == 2 && gamearr[1][2] == 2 && gamearr[2][2] == 2 ||
								gamearr[2][0] == 2 && gamearr[1][1] == 2 && gamearr[0][2] == 2)
							{
								winner = 2;
								gameState = RESULT;
								init = true;
							}
						else if (!nozero) {
							init = true;
							gameState = RESULT;
							winner = 0;
						}
						}
						
					}
						break;
					case ABOUTUS:
						break;
					case PAUSE:
						break;
					case RESULT:
						break;
					}
				}
			}
			window->clear();
			window->draw(background);//Drawing the background
			
			//Drawing items according to state
			switch (gameState)
			{
			case MENU: {
				gui.draw();
			}break;
			case GAME:
			{
				window->draw(yellowBack);
				window->draw(v1);
				window->draw(v2);
				window->draw(h1);
				window->draw(h2);
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++)
					{
						if (gamearr[i][j] == 1) {
							cross.setPosition(shapepostion.x + 25 + 70 * i, shapepostion.y + 25 + 70 * j);
							window->draw(cross);
						}
						else if(gamearr[i][j] == 2){
							oshape.setPosition(shapepostion.x  + 33 + 70 * i, shapepostion.y + 33 + 70 * j);
							window->draw(oshape);
						}

					}
				}
				gui.draw();
			}
				break;
			case ABOUTUS:
				gui.draw();
				break;
			case PAUSE:
				break;
			case RESULT:
				window->draw(yellowBack);
				window->draw(v1);
				window->draw(v2);
				window->draw(h1);
				window->draw(h2);
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++)
					{
						if (gamearr[i][j] == 1) {
							cross.setPosition(shapepostion.x + 25 + 70 * i, shapepostion.y + 25 + 70 * j);
							window->draw(cross);
						}
						else if (gamearr[i][j] == 2) {
							oshape.setPosition(shapepostion.x + 33 + 70 * i, shapepostion.y + 33 + 70 * j);
							window->draw(oshape);
						}

					}
				}
				gui.draw();
				break;
			}
			window->display();
		}
	}

};


