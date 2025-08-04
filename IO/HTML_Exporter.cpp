#include "HTML_Exporter.h"
#include "logMessage.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <cctype>

std::string HTML_Exporter::toCamelCase(const std::string& input) {
    std::stringstream ss(input);
    std::string word, result;
    bool first = true;

    while (ss >> word) {
        if (word.empty()) continue;

        if (first) {
            // minuscolo la prima parola
            word[0] = std::tolower(word[0]);
            result += word;
            first = false;
        } else {
            // maiuscola le successive
            word[0] = std::toupper(word[0]);
            result += word;
        }
    }

    return result;
}

void HTML_Exporter::saveHTML(const Concert &c, const std::string &filename) {
	std::setlocale(LC_TIME, "de_DE.UTF-8"); // per data in tedesco
	std::vector<MusicalPiece> p = c.getProgram();
	std::vector<Musician> m = c.getMusicians();
	std::vector<Rehearsal> r = c.getRehearsals();
	std::string rootDir = toCamelCase(c.getTitle());

	std::ofstream file(filename);
	if (!file) {
		LOG_MSG("Errore nello scrivere il file.");
		return;
	}

	file << "<!DOCTYPE HTML_Exporter>\n";
	file << "<HTML_Exporter lang=\"de\">\n";
	file << "\n";
	file << "<head>\n";
	file << "	<meta charset=\"UTF-8\">\n";
	file << "	<title>" << c.getTitle() << "a</title>\n";
	file << "	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
	file
			<< "	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
	file
			<< "	<link href=\"https://fonts.googleapis.com/css2?family=Open+Sans:wght@400;600&display=swap\" rel=\"stylesheet\">\n";
	file << "\n";
	file << "	<link rel=\"stylesheet\" href=\"style.css\">\n";
	file << "</head>\n";
	file << "\n";
	file << "<div id=\"container\">\n";
	file << "	<h1>" << c.getTitle() << "</h1>\n";
	file << "	<table>\n";
	file << "		<tr class=\"info-row\">\n";
	file << "			<td class=\"bold\">Werk</td>\n";

// Raggruppiamo le opere per compositore+date (stringa unica)
	std::map<std::string, std::vector<std::string>> worksByComposer;
	for (const auto &piece : p) {
		worksByComposer[piece.getComposer()].push_back(piece.getTitle());
	}

// Genera le celle con compositori e titoli
	file << "<td>\n";
	for (const auto& [composerWithDates, titles] : worksByComposer) {
		file << "			    " << composerWithDates << "<br>\n";
		file << "				<ul>\n";
		for (const auto &title : titles) {
			file << "					<li><i>" << title << "</i><br></li>\n";
		}
		file << "				</ul>\n";

	}
	file << "			</td>\n";
	file << "		</tr>\n";
	file << "\n";
	file << "\n";

// FLyer
	file << "<tr class=\"info-row\">\n";
	file << "    <td class=\"bold\">Flyer</td>\n";
	file << "    <td>\n";
	file << "        <?php\n";
	file << "        $flyerDir = '" << rootDir << "/Flyer';\n";
	file << "        $files = glob($flyerDir . '/*.pdf');\n";
	file << "        foreach ($files as $file) {\n";
	file << "            $fileName = basename($file);\n";
	file << "            echo \"<a href='$file'>$fileName</a><br>\";\n";
	file << "        }\n";
	file << "        ?>\n";
	file << "    </td>\n";
	file << "</tr>\n\n";

// Noten
	file << "<tr class=\"info-row\">\n";
	file << "    <td class=\"bold\">Noten</td>\n";
	file << "    <td>\n";
	file << "        <?php\n";
	file << "        $notenDir = '" << rootDir << "/Noten';\n";
	file
			<< "        $pieces = array_filter(scandir($notenDir), function($item) use ($notenDir) {\n";
	file
			<< "            return $item !== '.' && $item !== '..' && is_dir($notenDir . '/' . $item);\n";
	file << "        });\n";
	file << "        if(empty($pieces)) {\n";
	file
			<< "            // Se non ci sono sottocartelle, elenca i PDF direttamente\n";
	file << "            $files = glob($notenDir . '/*.pdf');\n";
	file << "            foreach($files as $file) {\n";
	file << "                $fileName = basename($file);\n";
	file << "                echo \"<a href='$file'>$fileName</a><br>\";\n";
	file << "            }\n";
	file << "        } else {\n";
	file << "            foreach($pieces as $piece) {\n";
	file
			<< "                echo \"<p><strong>$piece</strong></p>\\n<ul>\\n\";\n";
	file
			<< "                $files = glob($notenDir . '/' . $piece . '/*.pdf');\n";
	file << "                foreach($files as $file) {\n";
	file << "                    $fileName = basename($file);\n";
	file
			<< "                    echo \"<li><a href='$file'>$fileName</a></li>\\n\";\n";
	file << "                }\n";
	file << "                echo \"</ul>\\n\";\n";
	file << "            }\n";
	file << "        }\n";
	file << "        ?>\n";
	file << "    </td>\n";
	file << "</tr>\n\n\n\n";

	// Proben
	file << "		<tr class=\"info-row\">\n";
	file << "			<td class=\"bold\">Proben</td>\n";
	file << "			<td>\n";
	file << "				<ul>\n";



	for (const auto &rehearsal : r) {
		std::tm tm = rehearsal.getDate();
		char buffer[100];
		std::strftime(buffer, sizeof(buffer), "%A, %d. %B %Y", &tm);

		std::string startTime = rehearsal.getStartTime();

		file << "					<li>" << buffer << " - " << startTime << " Uhr";
		if (!rehearsal.getMusicians().empty()) {
			file << " <strong>(" << rehearsal.getMusicians() << ")</strong>";
		}
		file << "</li>\n";
	}

	file << "				</ul>\n";
	file << "			</td>\n";
	file << "		</tr>\n\n";

// Konzerte
	file << "		<tr class=\"info-row\">\n";
	file << "			<td class=\"bold\">Aufführung</td>\n";
	file << "			<td>\n";
	file << "				<ul>\n";

	const auto &dates = c.getDatesAsTm();
	std::setlocale(LC_TIME, "de_DE.UTF-8"); // per il formato tedesco

	for (const auto &date : dates) {
		char buffer[100];
		// Format: "Freitag, 18. April 2025"
		std::strftime(buffer, sizeof(buffer), "%A, %d. %B %Y", &date);
		file << "					<li>" << buffer << " Uhr</li>\n";
	}

	file << "				</ul>\n";
	file << "			</td>\n";
	file << "		</tr>\n\n";

	file << "			<tr class=\"info-row\">\n";
	file << "			<td class=\"bold\">Tenue</td>\n";
	file << "			<td>Schwarz - schwarz</td>\n";
	file << "			</tr>\n";
	file << "			\n";
	file << "			<tr class=\"info-row\">\n";
	file << "			<td class=\"bold\">Gage</td>\n";
	file << "			<td>- CHF plus Spesen (Öv, 2. Kl, 1/2-Abo)</td>\n";
	file << "			</tr>\n";
	file << "		\n";
	file << "		<tr class=\"info-row\">\n";
	file << "                <td class=\"bold\">Stimmung:</td>\n";
	file << "                <td>415 Hz</td>\n";
	file << "            </tr>\n";
	file << "            \n";
	file << "		<tr class=\"info-row\">\n";
	file << "                <td class=\"bold\">Adresse:</td>\n";
	file << "                <td>\n";
	file << "                    bitte füllen<br>\n";
	file << "                    bitte füllen<br>\n";
	file << "                    bitte füllen<br>\n";
	file << "                </td>\n";
	file << "            </tr>\n";
	file << "	<tr class=\"info-row\">\n";
	file << "	<td class=\"bold\">Korrekturen:</td>\n";
	file << "	<td><a href=\"Changelog.pdf\">Hier</a> clicken.</td>\n";
	file << "      </tr>\n";
	file << "	</table>\n";
	file << "	\n";
	file << "	        <h3>Telefonliste</h3>\n";
	file << "\n";
	file << "        <table id=\"adress-liste\">\n";
	file << "            <tr>\n";
	file << "                <th>Name</th>\n";
	file << "                <th>Instrument</th>\n";
	file << "                <th>Telefonnummer</th>\n";
	file << "                <th>Mailadresse</th>\n";
	file << "            </tr>\n";
	file << "            <tr>\n";
	file << "                <td>Eugenio Giovine</td>\n";
	file << "                <td>Dirigent</td>\n";
	file << "                <td>077 418 76 18</td>\n";
	file << "                <td>eugenio.giovine@reformiert-zuerich.ch</td>\n";
	file << "            </tr>\n";
	for (const Musician &musician : c.getMusicians()) {
		file << "            <tr>\n";
		file << "                <td>" << musician.getName() << "</td>\n";
		file << "                <td>" << musician.getInstrument() << "</td>\n";
		file << "                <td>" << musician.getPhone() << "</td>\n";
		file << "                <td>" << musician.getMail() << "</td>\n";
		file << "            </tr>\n";
	}
	file << "       </table>\n";
	file << "\n";
	file << "        \n";
	file << "        \n";
	file << "</div>\n";
	file.close();

}
