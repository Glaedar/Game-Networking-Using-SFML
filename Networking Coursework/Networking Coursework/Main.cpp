#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<string>
#include<iostream>

using namespace std;

class Player
{
public:
	sf::RectangleShape rect;
	float bottom, left, right, top;

	Player(sf::Vector2f position, sf::Vector2f size, sf::Color color)
	{
		rect.setPosition(position);
		rect.setSize(size);
		rect.setFillColor(color);
	}

	void update()
	{
		bottom = rect.getPosition().y + rect.getSize().y;
		left = rect.getPosition().x;
		right = rect.getPosition().x + rect.getSize().x;
		top = rect.getPosition().y;
	}

	bool Collision(Player p)
	{
		if (right < p.left || left > p.right || top > p.bottom || bottom < p.top)
		{
			return false;
		}
		return true;
	}
};

class Pickup
{
	public:
	sf::RectangleShape rect;
	float bottom, left, right, top;

	Pickup(sf::Vector2f position, sf::Vector2f size, sf::Color color)
	{
		rect.setPosition(position);
		rect.setSize(size);
		rect.setFillColor(color);
	}

	void update()
	{
		bottom = rect.getPosition().y + rect.getSize().y;
		left = rect.getPosition().x;
		right = rect.getPosition().x + rect.getSize().x;
		top = rect.getPosition().y;
	}

	bool Collision(Player p)
	{
		if (right < p.left || left > p.right || top > p.bottom || bottom < p.top)
		{
			return false;
		}
		return true;
	}
};


int main()
{
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();

	sf::TcpSocket socket;
	char connectionType, mode;

	char buffer[2000];
	std::size_t received;

	std::string text = "Connected to: ";

	std::cout << "Enter (s) for server, Enter (c) for client: " << std::endl;
	cin >> connectionType;

	if (connectionType == 's')
	{
		sf::TcpListener listener;
		listener.listen(2000);
		listener.accept(socket);
		text += "Server";
		socket.send(text.c_str(), text.length() + 1);
		//mode = 's';
	}

	else if (connectionType == 'c')
	{
		socket.connect(ip, 2000);
		text += "Client";
		socket.send(text.c_str(), text.length() + 1);
		//mode = 'r';
	}

	socket.receive(buffer, sizeof(buffer), received);
	std::cout << buffer << std::endl;

	//sf::RectangleShape player1, player2, enemy;

	Player player1(Player(sf::Vector2f(0, 0), sf::Vector2f(20, 20), sf::Color::Red));
	Player player2(Player(sf::Vector2f(0, 0), sf::Vector2f(20, 20), sf::Color::Blue));

	Pickup pickup1(Pickup(sf::Vector2f(200, 50), sf::Vector2f(20, 20), sf::Color::Yellow));
	Pickup pickup2(Pickup(sf::Vector2f(200, 300), sf::Vector2f(20, 20), sf::Color::Yellow));
	Pickup pickup3(Pickup(sf::Vector2f(200, 500), sf::Vector2f(20, 20), sf::Color::Yellow));
	Pickup pickup4(Pickup(sf::Vector2f(400, 300), sf::Vector2f(20, 20), sf::Color::Yellow));
	Pickup pickup5(Pickup(sf::Vector2f(600, 150), sf::Vector2f(20, 20), sf::Color::Yellow));
	Pickup pickup6(Pickup(sf::Vector2f(700, 450), sf::Vector2f(20, 20), sf::Color::Yellow));
	//float bottom, left, right, up;

	int score = 0;
	int score2 = 0;

	sf::Font font;

	if (!font.loadFromFile("arialbd.ttf"))
	{
		std::cout << "can't find font" << std::endl;
	}

	sf::Text playerScore;
	sf::Text opponentScore;

	playerScore.setString("Your score is: " + score);
	opponentScore.setString("Your score is: " + score2);
	playerScore.setCharacterSize(24);
	opponentScore.setCharacterSize(24);
	playerScore.setFillColor(sf::Color::Red);
	opponentScore.setFillColor(sf::Color::Blue);

	playerScore.setStyle(sf::Text::Bold | sf::Text::Underlined);
	opponentScore.setStyle(sf::Text::Bold | sf::Text::Underlined);

	//player1.setSize(sf::Vector2f(20, 20));
	//player2.setSize(sf::Vector2f(20, 20));
	//enemy.setSize(sf::Vector2f(20, 20));


	//player1.setFillColor(sf::Color::Red);
	//player2.setFillColor(sf::Color::Blue);
	//enemy.setFillColor(sf::Color::Green);

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Networking Coursework");

	sf::Vector2f prevPosition, p2Position, enemyPosition;


	socket.setBlocking(false);

	bool update = false;

	while (window.isOpen())
	{

		sf::Event Event;
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed || Event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
			else if (Event.type == sf::Event::GainedFocus)
			{
				update = true;
			}
			else if (Event.type == sf::Event::LostFocus)
			{
				update = false;
			}
		}


		prevPosition = player1.rect.getPosition();


		if (update)
		{
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				player1.rect.move(0.1f, 0.0f);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				player1.rect.move(-0.1f, 0.0f);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				player1.rect.move(0.0f, 0.1f);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				player1.rect.move(0.0f, -0.1f);
			}
		}
		sf::Packet packet;

		if (prevPosition != player1.rect.getPosition())
		{
			packet << player1.rect.getPosition().x << player1.rect.getPosition().y;
			socket.send(packet);
		}

		socket.receive(packet);

		//if packet does
		if (packet >> p2Position.x >> p2Position.y)
		{
			player2.rect.setPosition(p2Position);
		}

		//player updates
		player1.update();
		player2.update();

		//pickup updates
		pickup1.update();
		pickup2.update();
		pickup3.update();
		pickup4.update();
		pickup5.update();
		pickup6.update();


		//player scores
		if (pickup1.Collision(player1))
		{
			score++;
		}
		if (pickup2.Collision(player1))
		{
			score++;
		}
		if (pickup3.Collision(player1))
		{
			score++;
		}
		if (pickup4.Collision(player1))
		{
			score++;
		}
		if (pickup5.Collision(player1))
		{
			score++;
		}
		if (pickup6.Collision(player1))
		{
			score++;
		}

		//opponent scores
		if (pickup1.Collision(player2))
		{
			score2++;
		}
		if (pickup2.Collision(player2))
		{
			score2++;
		}
		if (pickup3.Collision(player2))
		{
			score2++;
		}
		if (pickup4.Collision(player2))
		{
			score2++;
		}
		if (pickup5.Collision(player2))
		{
			score2++;
		}
		if (pickup6.Collision(player2))
		{
			score2++;
		}

		//draw to window
		window.draw(player1.rect);
		window.draw(player2.rect);
		window.draw(pickup1.rect);
		window.draw(pickup2.rect);
		window.draw(pickup3.rect);
		window.draw(pickup4.rect);
		window.draw(pickup5.rect);
		window.draw(pickup6.rect);
	
		//window.draw(enemy);
		window.draw(playerScore);
		window.draw(opponentScore);
		window.display();
		window.clear();
	
		

	}


	//system("pause");

	return 0;
}
