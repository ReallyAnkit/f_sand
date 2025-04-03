#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <vector>

constexpr unsigned int WIDTH=1600,HEIGHT=800;
const float gridSizef = 5.0f;
int rows = HEIGHT / gridSizef;
int cols = WIDTH / gridSizef;
sf::Color bgColor = sf::Color::Black , sandColor = sf::Color::White;


using Grid = std::vector<std::vector<sf::Color>>;

sf::Color randomColor(){
	return sf::Color(rand() % 255 , rand() % 255 ,rand() % 255);
}


void drawGrid(Grid& grid , sf::RenderWindow& window)
{
	sf::VertexArray vertices(sf::Quads);
	for(int i = 0 ; i < rows; i++){
		for(int j = 0 ; j < cols; j++){
			if(grid[i][j] != bgColor){
			sf::Vertex quads[4];
			quads[0].position = sf::Vector2f(j*gridSizef , i*gridSizef);
			quads[1].position = sf::Vector2f(j*gridSizef + gridSizef , i*gridSizef);
			quads[2].position = sf::Vector2f(j*gridSizef + gridSizef, i*gridSizef + gridSizef);
			quads[3].position = sf::Vector2f(j*gridSizef , i*gridSizef+gridSizef);

				for(auto& q : quads) q.color = grid[i][j];
				for(auto& q : quads) vertices.append(q); 
			}
		}
	}

	window.draw(vertices);
	vertices.clear();	
}
void randomizeRow(Grid& grid)
{
	grid[0][rand() % cols] = sandColor;
}
void updateGrid(sf::Vector2i mPos , Grid& grid , bool flag )
{
	int i = std::clamp(mPos.y / gridSizef, 0.0f, rows - 1.f);
    int j = std::clamp(mPos.x / gridSizef, 0.0f, cols - 1.f);

	for(int idx = std::max(0, i-3) ; idx < std::min((int)rows , i+3) ; idx++){
		for(int j_idx = std::max(0, j-3) ; j_idx < std::min((int)cols , j+3) ; j_idx++)
			if(rand() % 7 == 0 )
				grid[idx][j_idx] = sandColor;
	}	
}
void fallingSand(Grid& grid){
	for(int i = grid.size()-1 ; i > 0 ; i--){
		for(int j = grid[0].size()-1 ; j >= 0; j--){

			if(grid[i-1][j] != bgColor)
			{
				if(grid[i][j] != bgColor)
				{
					if(grid[i][j-1] == bgColor){
						grid[i][j-1] = grid[i-1][j];
						grid[i-1][j] = bgColor; 
					}
					else if(grid[i][j+1] == bgColor ){
						grid[i][j+1] = grid[i-1][j];
						grid[i-1][j] = bgColor; 
					}
				}
				else{
					grid[i][j] = grid[i-1][j];
					grid[i-1][j] = bgColor; 
				}
			}
		}
	}
}

void setGrid(Grid& grid)
{
	for(auto &row : grid){
		for(auto &block : row){
			block = bgColor;
		}
	}
}

int main(void){
	srand(time(0));
	sf::ContextSettings settings;
	sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT) , "PathFinding" , sf::Style::Default , settings);
	Grid grid(rows , std::vector<sf::Color>(cols));
	setGrid(grid);
	sf::Clock clock;
	window.setFramerateLimit(60);

	while(window.isOpen())
	{
		float eTime = clock.getElapsedTime().asSeconds();
		sf::Event event;

		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed) window.close();
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				updateGrid(sf::Mouse::getPosition(window) , grid , true); 
			}
		}
		window.clear();
		randomizeRow(grid);
		drawGrid(grid,window);
		fallingSand(grid);
		window.display();
		if( eTime > 5.0f){
			sandColor = randomColor(); 
			clock.restart();
		}
	}
}