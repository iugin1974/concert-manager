#include "Model/Concert.h"
#include "Model/Musician.h"
#include "Controller/ConcertController.h"
#include "Core/logMessage.h"
#include "IO/HTML.h"

int main() {
	Concert c;
	c.setTitle("Title");
	c.addDate("10.03.2025");
	c.addDate("11.03.2025");
Musician m;
	m.setName("Eugenio GIovine");
	m.setPhone("098123 3457 8");

	c.addMusician(m);
	HTML h;
	h.saveHTML(c);

	return 0;
}
