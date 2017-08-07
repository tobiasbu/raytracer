
class IEvent {

public:

	struct Key 
	{

		bool system;
		bool alt;
		bool control;
		bool shift;
	};

	struct Text 
	{
		unsigned int unicode;
	};

	enum Type {
		KeyPressed,
		KeyReleased,
		TextEntered
	};

};