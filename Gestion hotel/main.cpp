#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cstdlib>

using namespace std;

/* ================= COLORS ================= */

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define WHITE   "\033[37m"
#define GOLD    "\033[38;5;220m"
#define GRAY    "\033[38;5;245m"

/* ================= STRUCTURES ================= */

struct Client {
    int id;
    string nom;
    string prenom;
    string telephone;
    string email;
};

struct Chambre {
    int numero;
    string type;
    double prix;
    bool disponible;
};

struct Reservation {
    int id;
    int clientId;
    int numeroChambre;
    string dateArrivee;
    string dateDepart;
    bool active;
};

/* ================= GLOBAL DATA ================= */

vector<Client> clients;
vector<Chambre> chambres;
vector<Reservation> reservations;

int nextClientId = 1;
int nextReservationId = 1;

/* ================= UI TOOLS ================= */

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string cutText(string text, int maxLen) {
    if ((int)text.size() <= maxLen) return text;
    if (maxLen <= 3) return text.substr(0, maxLen);
    return text.substr(0, maxLen - 3) + "...";
}

void pauseScreen() {
    cout << endl;
    cout << GRAY << "  Press ENTER pour continuer..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void hr(int width = 70, char c = '-') {
    cout << GRAY << "  ";
    for (int i = 0; i < width; i++) cout << c;
    cout << RESET << endl;
}

void topBox(int width) {
    cout << CYAN << BOLD << "  +";
    for (int i = 0; i < width - 2; i++) cout << "-";
    cout << "+" << RESET << endl;
}

void midBox(int width) {
    cout << CYAN << BOLD << "  +";
    for (int i = 0; i < width - 2; i++) cout << "-";
    cout << "+" << RESET << endl;
}

void bottomBox(int width) {
    cout << CYAN << BOLD << "  +";
    for (int i = 0; i < width - 2; i++) cout << "-";
    cout << "+" << RESET << endl;
}

void boxLine(const string& text, int width, const string& color = WHITE) {
    string t = cutText(text, width - 6);
    cout << CYAN << BOLD << "  | " << RESET;
    cout << color << left << setw(width - 4) << t << RESET;
    cout << CYAN << BOLD << "|" << RESET << endl;
}

void centerBoxLine(const string& text, int width, const string& color = WHITE) {
    int inner = width - 4;
    string t = cutText(text, inner);
    int leftPad = (inner - (int)t.size()) / 2;
    int rightPad = inner - (int)t.size() - leftPad;

    cout << CYAN << BOLD << "  |" << RESET;
    cout << string(leftPad, ' ');
    cout << color << BOLD << t << RESET;
    cout << string(rightPad, ' ');
    cout << CYAN << BOLD << "|" << RESET << endl;
}

void printHeader(const string& page) {
    clearScreen();
    cout << endl;

    topBox(74);
    centerBoxLine("HOTEL MANAGEMENT SYSTEM", 74, GOLD);
    centerBoxLine("GRAND HOTEL DU MAGHREB", 74, WHITE);
    midBox(74);
    centerBoxLine(page, 74, YELLOW);
    bottomBox(74);

    cout << endl;
}

void printMenu(const string& title, const vector<string>& options) {
    cout << MAGENTA << BOLD << "  " << title << RESET << endl;
    hr(58, '=');

    for (int i = 0; i < (int)options.size(); i++) {
        cout << "  " << CYAN << "[" << i + 1 << "] " << RESET
             << WHITE << left << setw(38) << options[i] << RESET << endl;
    }

    cout << "  " << RED << "[0] " << RESET
         << WHITE << left << setw(38) << "Retour / Quitter" << RESET << endl;

    hr(58, '=');
    cout << GOLD << BOLD << "  Votre choix > " << RESET;
}

void sectionTitle(const string& title) {
    cout << endl;
    cout << MAGENTA << BOLD << "  " << title << RESET << endl;
    hr(70, '-');
}

void successMsg(const string& msg) {
    cout << endl << GREEN << BOLD << "  [SUCCESS] " << RESET << GREEN << msg << RESET << endl;
}

void errorMsg(const string& msg) {
    cout << endl << RED << BOLD << "  [ERROR] " << RESET << RED << msg << RESET << endl;
}

void infoMsg(const string& msg) {
    cout << BLUE << BOLD << "  [INFO] " << RESET << msg << endl;
}

string getInput(const string& prompt) {
    string val;
    cout << "  " << CYAN << prompt << RESET << " : ";
    getline(cin, val);
    return val;
}

int getInt(const string& prompt) {
    int val;
    while (true) {
        if (!prompt.empty()) {
            cout << "  " << CYAN << prompt << RESET << " : ";
        }

        cin >> val;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            errorMsg("Veuillez saisir un nombre valide.");
            if (prompt.empty()) cout << GOLD << BOLD << "  Votre choix > " << RESET;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
    }
}

double getDouble(const string& prompt) {
    double val;
    while (true) {
        cout << "  " << CYAN << prompt << RESET << " : ";
        cin >> val;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            errorMsg("Veuillez saisir un prix valide.");
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
    }
}

/* ================= INIT ================= */

void initDonnees() {
    chambres.push_back({101, "Simple", 350.0, true});
    chambres.push_back({102, "Simple", 350.0, true});
    chambres.push_back({201, "Double", 600.0, true});
    chambres.push_back({202, "Double", 600.0, true});
    chambres.push_back({301, "Suite", 1200.0, true});
    chambres.push_back({302, "Suite", 1200.0, false});
    chambres.push_back({401, "Deluxe", 900.0, true});
    chambres.push_back({402, "Deluxe", 900.0, true});
}

/* ================= FIND HELPERS ================= */

Client* findClientById(int id) {
    for (Client& c : clients) {
        if (c.id == id) return &c;
    }
    return nullptr;
}

Chambre* findChambreByNumero(int numero) {
    for (Chambre& ch : chambres) {
        if (ch.numero == numero) return &ch;
    }
    return nullptr;
}

/* ================= TABLES ================= */

void tableClients() {
    if (clients.empty()) {
        infoMsg("Aucun client enregistre.");
        return;
    }

    cout << endl;
    cout << GOLD << BOLD;
    cout << "  +------+------------------------+----------------+------------------------------+" << endl;
    cout << "  | ID   | Nom complet            | Telephone      | Email                        |" << endl;
    cout << "  +------+------------------------+----------------+------------------------------+" << endl;
    cout << RESET;

    for (const Client& c : clients) {
        string full = cutText(c.prenom + " " + c.nom, 22);
        string tel = cutText(c.telephone, 14);
        string mail = cutText(c.email, 28);

        cout << "  | " << CYAN << left << setw(4) << c.id << RESET
             << " | " << WHITE << left << setw(22) << full << RESET
             << " | " << WHITE << left << setw(14) << tel << RESET
             << " | " << WHITE << left << setw(28) << mail << RESET
             << " |" << endl;
    }

    cout << GOLD << BOLD;
    cout << "  +------+------------------------+----------------+------------------------------+" << endl;
    cout << RESET;
}

void tableChambres(bool onlyAvailable = false) {
    bool found = false;

    cout << endl;
    cout << CYAN << BOLD;
    cout << "  +----------+--------------+------------+---------------+" << endl;
    cout << "  | Numero   | Type         | Prix/Nuit  | Statut        |" << endl;
    cout << "  +----------+--------------+------------+---------------+" << endl;
    cout << RESET;

    for (const Chambre& ch : chambres) {
        if (onlyAvailable && !ch.disponible) continue;
        found = true;

        cout << "  | " << WHITE << left << setw(8) << ch.numero << RESET
             << " | " << WHITE << left << setw(12) << cutText(ch.type, 12) << RESET
             << " | " << YELLOW << left << setw(10) << fixed << setprecision(0) << ch.prix << RESET
             << " | ";

        if (ch.disponible)
            cout << GREEN << left << setw(13) << "Libre" << RESET;
        else
            cout << RED << left << setw(13) << "Occupee" << RESET;

        cout << " |" << endl;
    }

    cout << CYAN << BOLD;
    cout << "  +----------+--------------+------------+---------------+" << endl;
    cout << RESET;

    if (!found) infoMsg("Aucune chambre trouvee.");
}

void tableReservations() {
    bool found = false;

    cout << endl;
    cout << MAGENTA << BOLD;
    cout << "  +------+----------+------------------------+--------------+--------------+" << endl;
    cout << "  | ID   | Chambre  | Client                 | Arrivee      | Depart       |" << endl;
    cout << "  +------+----------+------------------------+--------------+--------------+" << endl;
    cout << RESET;

    for (const Reservation& r : reservations) {
        if (!r.active) continue;
        found = true;

        string nomClient = "Inconnu";
        for (const Client& c : clients) {
            if (c.id == r.clientId) {
                nomClient = c.prenom + " " + c.nom;
                break;
            }
        }

        cout << "  | " << CYAN << left << setw(4) << r.id << RESET
             << " | " << WHITE << left << setw(8) << r.numeroChambre << RESET
             << " | " << WHITE << left << setw(22) << cutText(nomClient, 22) << RESET
             << " | " << WHITE << left << setw(12) << cutText(r.dateArrivee, 12) << RESET
             << " | " << WHITE << left << setw(12) << cutText(r.dateDepart, 12) << RESET
             << " |" << endl;
    }

    cout << MAGENTA << BOLD;
    cout << "  +------+----------+------------------------+--------------+--------------+" << endl;
    cout << RESET;

    if (!found) infoMsg("Aucune reservation active.");
}

/* ================= CLIENTS ================= */

void gestionClients() {
    int choix;

    do {
        printHeader("GESTION DES CLIENTS");

        vector<string> opts;
        opts.push_back("Ajouter un client");
        opts.push_back("Afficher tous les clients");
        opts.push_back("Rechercher un client");
        opts.push_back("Modifier un client");
        opts.push_back("Supprimer un client");

        printMenu("CLIENTS", opts);
        choix = getInt("");

        if (choix == 1) {
            printHeader("NOUVEAU CLIENT");

            Client c;
            c.id = nextClientId++;
            c.nom = getInput("Nom");
            c.prenom = getInput("Prenom");
            c.telephone = getInput("Telephone");
            c.email = getInput("Email");

            clients.push_back(c);
            successMsg("Client ajoute avec ID : " + to_string(c.id));
            pauseScreen();

        } else if (choix == 2) {
            printHeader("LISTE DES CLIENTS");
            tableClients();
            pauseScreen();

        } else if (choix == 3) {
            printHeader("RECHERCHER CLIENT");

            string terme = getInput("Nom ou prenom");
            bool trouve = false;

            for (const Client& c : clients) {
                if (c.nom.find(terme) != string::npos ||
                    c.prenom.find(terme) != string::npos) {
                    cout << endl;
                    topBox(58);
                    boxLine("ID        : " + to_string(c.id), 58, CYAN);
                    boxLine("Nom       : " + c.prenom + " " + c.nom, 58, WHITE);
                    boxLine("Telephone : " + c.telephone, 58, WHITE);
                    boxLine("Email     : " + c.email, 58, WHITE);
                    bottomBox(58);
                    trouve = true;
                }
            }

            if (!trouve) errorMsg("Aucun client trouve.");
            pauseScreen();

        } else if (choix == 4) {
            printHeader("MODIFIER CLIENT");

            int id = getInt("ID du client");
            Client* c = findClientById(id);

            if (c == nullptr) {
                errorMsg("Client introuvable.");
            } else {
                infoMsg("Laissez vide pour garder l'ancienne valeur.");
                string tmp;

                tmp = getInput("Nouveau nom [" + c->nom + "]");
                if (!tmp.empty()) c->nom = tmp;

                tmp = getInput("Nouveau prenom [" + c->prenom + "]");
                if (!tmp.empty()) c->prenom = tmp;

                tmp = getInput("Nouveau telephone [" + c->telephone + "]");
                if (!tmp.empty()) c->telephone = tmp;

                tmp = getInput("Nouvel email [" + c->email + "]");
                if (!tmp.empty()) c->email = tmp;

                successMsg("Client modifie.");
            }

            pauseScreen();

        } else if (choix == 5) {
            printHeader("SUPPRIMER CLIENT");

            int id = getInt("ID du client a supprimer");

            auto it = find_if(clients.begin(), clients.end(),
                              [id](const Client& c) { return c.id == id; });

            if (it != clients.end()) {
                clients.erase(it);
                successMsg("Client supprime.");
            } else {
                errorMsg("Client introuvable.");
            }

            pauseScreen();

        } else if (choix != 0) {
            errorMsg("Option invalide.");
            pauseScreen();
        }

    } while (choix != 0);
}

/* ================= CHAMBRES ================= */

void gestionChambres() {
    int choix;

    do {
        printHeader("GESTION DES CHAMBRES");

        vector<string> opts;
        opts.push_back("Afficher toutes les chambres");
        opts.push_back("Afficher les chambres disponibles");
        opts.push_back("Ajouter une chambre");
        opts.push_back("Modifier le prix d'une chambre");

        printMenu("CHAMBRES", opts);
        choix = getInt("");

        if (choix == 1) {
            printHeader("TOUTES LES CHAMBRES");
            tableChambres(false);
            pauseScreen();

        } else if (choix == 2) {
            printHeader("CHAMBRES DISPONIBLES");
            tableChambres(true);
            pauseScreen();

        } else if (choix == 3) {
            printHeader("AJOUTER CHAMBRE");

            Chambre ch;
            ch.numero = getInt("Numero de chambre");
            ch.type = getInput("Type Simple/Double/Suite/Deluxe");
            ch.prix = getDouble("Prix par nuit MAD");
            ch.disponible = true;

            chambres.push_back(ch);
            successMsg("Chambre ajoutee.");
            pauseScreen();

        } else if (choix == 4) {
            printHeader("MODIFIER PRIX");

            int num = getInt("Numero de chambre");
            Chambre* ch = findChambreByNumero(num);

            if (ch == nullptr) {
                errorMsg("Chambre introuvable.");
            } else {
                ch->prix = getDouble("Nouveau prix MAD");
                successMsg("Prix mis a jour.");
            }

            pauseScreen();

        } else if (choix != 0) {
            errorMsg("Option invalide.");
            pauseScreen();
        }

    } while (choix != 0);
}

/* ================= RESERVATIONS ================= */

void gestionReservations() {
    int choix;

    do {
        printHeader("GESTION DES RESERVATIONS");

        vector<string> opts;
        opts.push_back("Nouvelle reservation");
        opts.push_back("Afficher toutes les reservations");
        opts.push_back("Annuler une reservation");
        opts.push_back("Rechercher par client");

        printMenu("RESERVATIONS", opts);
        choix = getInt("");

        if (choix == 1) {
            printHeader("NOUVELLE RESERVATION");

            int cid = getInt("ID du client");
            Client* c = findClientById(cid);

            if (c == nullptr) {
                errorMsg("Client introuvable.");
                pauseScreen();
                continue;
            }

            infoMsg("Client selectionne : " + c->prenom + " " + c->nom);
            tableChambres(true);

            int num = getInt("Numero de chambre");
            Chambre* ch = findChambreByNumero(num);

            if (ch == nullptr || !ch->disponible) {
                errorMsg("Chambre indisponible ou introuvable.");
            } else {
                Reservation r;
                r.id = nextReservationId++;
                r.clientId = cid;
                r.numeroChambre = num;
                r.dateArrivee = getInput("Date arrivee JJ/MM/AAAA");
                r.dateDepart = getInput("Date depart JJ/MM/AAAA");
                r.active = true;

                reservations.push_back(r);
                ch->disponible = false;

                successMsg("Reservation creee avec ID : " + to_string(r.id));
            }

            pauseScreen();

        } else if (choix == 2) {
            printHeader("TOUTES LES RESERVATIONS");
            tableReservations();
            pauseScreen();

        } else if (choix == 3) {
            printHeader("ANNULER RESERVATION");

            int id = getInt("ID de la reservation");
            bool trouve = false;

            for (Reservation& r : reservations) {
                if (r.id == id && r.active) {
                    r.active = false;

                    Chambre* ch = findChambreByNumero(r.numeroChambre);
                    if (ch != nullptr) ch->disponible = true;

                    trouve = true;
                    successMsg("Reservation annulee. Chambre liberee.");
                    break;
                }
            }

            if (!trouve) errorMsg("Reservation introuvable ou deja annulee.");
            pauseScreen();

        } else if (choix == 4) {
            printHeader("RECHERCHE PAR CLIENT");

            int cid = getInt("ID du client");
            bool found = false;

            for (const Reservation& r : reservations) {
                if (r.clientId == cid && r.active) {
                    infoMsg("Reservation #" + to_string(r.id) +
                            " | Chambre " + to_string(r.numeroChambre) +
                            " | " + r.dateArrivee + " -> " + r.dateDepart);
                    found = true;
                }
            }

            if (!found) infoMsg("Aucune reservation active pour ce client.");
            pauseScreen();

        } else if (choix != 0) {
            errorMsg("Option invalide.");
            pauseScreen();
        }

    } while (choix != 0);
}

/* ================= DASHBOARD ================= */

void afficherStatistiques() {
    printHeader("TABLEAU DE BORD");

    int total = (int)chambres.size();
    int occupees = 0;
    int libres = 0;
    int reservationsActives = 0;
    double revenu = 0.0;

    for (const Chambre& ch : chambres) {
        if (ch.disponible) libres++;
        else occupees++;
    }

    for (const Reservation& r : reservations) {
        if (r.active) {
            reservationsActives++;
            Chambre* ch = findChambreByNumero(r.numeroChambre);
            if (ch != nullptr) revenu += ch->prix;
        }
    }

    cout << GOLD << BOLD;
    cout << "  +----------------------------------------------------------+" << endl;
    cout << "  |                    RESUME GENERAL                       |" << endl;
    cout << "  +------------------------------+---------------------------+" << endl;
    cout << RESET;

    cout << "  | Total chambres               | " << CYAN << left << setw(25) << total << RESET << "|" << endl;
    cout << "  | Chambres libres              | " << GREEN << left << setw(25) << libres << RESET << "|" << endl;
    cout << "  | Chambres occupees            | " << RED << left << setw(25) << occupees << RESET << "|" << endl;
    cout << "  | Total clients                | " << CYAN << left << setw(25) << clients.size() << RESET << "|" << endl;
    cout << "  | Reservations actives         | " << YELLOW << left << setw(25) << reservationsActives << RESET << "|" << endl;
    cout << "  | Revenu estime                | " << GREEN << fixed << setprecision(0) << left << setw(20) << revenu << " MAD" << RESET << "|" << endl;

    cout << GOLD << BOLD;
    cout << "  +------------------------------+---------------------------+" << endl;
    cout << RESET;

    int taux = total > 0 ? (occupees * 100 / total) : 0;
    int bars = total > 0 ? (occupees * 34 / total) : 0;

    cout << endl << "  Taux occupation : ";
    cout << "[";
    cout << RED;
    for (int i = 0; i < bars; i++) cout << "#";
    cout << GREEN;
    for (int i = bars; i < 34; i++) cout << ".";
    cout << RESET << "] ";
    cout << YELLOW << BOLD << taux << "%" << RESET << endl;

    pauseScreen();
}

/* ================= MAIN ================= */

int main() {
    initDonnees();

    int choix;

    do {
        printHeader("MENU PRINCIPAL");

        vector<string> opts;
        opts.push_back("Gestion des Clients");
        opts.push_back("Gestion des Chambres");
        opts.push_back("Gestion des Reservations");
        opts.push_back("Statistiques et Tableau de Bord");

        printMenu("MENU", opts);
        choix = getInt("");

        switch (choix) {
            case 1:
                gestionClients();
                break;
            case 2:
                gestionChambres();
                break;
            case 3:
                gestionReservations();
                break;
            case 4:
                afficherStatistiques();
                break;
            case 0:
                clearScreen();
                cout << endl;
                topBox(60);
                centerBoxLine("MERCI D'AVOIR UTILISE NOTRE SYSTEME", 60, GOLD);
                centerBoxLine("A BIENTOT", 60, WHITE);
                bottomBox(60);
                cout << endl;
                break;
            default:
                errorMsg("Option invalide.");
                pauseScreen();
                break;
        }

    } while (choix != 0);

    return 0;
}
