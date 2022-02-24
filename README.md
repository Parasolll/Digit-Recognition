# HandWrittenDigitRecognition
Neural Network to recognize hand written digit, written in C.

*Neural Network sees a 4 with high confidence*
![example](https://user-images.githubusercontent.com/100319151/155535844-4c1b181a-0b84-459a-be9e-1ff612157652.PNG)
*Neural Network sees a 2 with high confidence*
![example2](https://user-images.githubusercontent.com/100319151/155535987-6d62d1b3-f541-47b0-9aed-e1cce3c187d1.PNG)


INFORMATIONS :

	This project was a way to learn about neural networks and deep learning.

	I decided to share the code so other people can learn from it,
	after a little cleaning of code, the UI and the addition of some comments, here it is.
	Please feel free to contact me if you have any question.

	Here is a little demo of the program I made:
	https://www.youtube.com/watch?v=w_xLc6b-7hg

	Training set and Testing set used are from THE MNIST DATABASE: 
	- http://yann.lecun.com/exdb/mnist/

	The Neural Network is pretty simple,it uses Deep Learning
	He is a cool playlist by 3Blue1Brown giving an introduction to Deep Learning:
	- https://youtube.com/playlist?list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi

COMPILE IT :

	I only built this project on windows with MinGW GCC and MSVC so i don't know if it will compile
	using other C compiler.

	All the source code can be found inside src and include.

	The "third_party" directory contains the SDL lib that is used for a quick and simple interface.


HOW TO USE IT :
	
	Button can be identified by their color:

	Beige      - Is the Training button, once you click on it
				 	the neural network will start training then testing.
				 	Because the program is single threaded the window
				 	of the app will be unresponsive for a couple of seconds.

	Purple     - Is the Submit button, once you click on it
				 	the drawing canvas will be used as an input for the neural network.
				 	The output of the neural network will be visible on the right
				 	Each line represent the probability(between 0 and 1) 
				 	of the drawing being a digit(between 0 and 9).

	Pink       - Is the clear button, it clear the drawing canvas.

	Dark Grey  - Is the pencil button, once you click on it you will be
				 	able to draw on the canvas.


	Light Grey - Is the eraser button, once you click on it you will be
				 	able to erase on the canvas.

	Green      - Is the smooth button, once you click on it
					each pixel in the drawing canvas will be "averaged" 
					using his neighbours to have something "smoother".

CONTACT :

	If you want to contact me or ask me question
	please feel free to do on twitter: @P4R4S0L
