package lammm;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class FauxPeer1 {
    private String peerId;
    public static void main(String[] args) {
        BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in));

        while (true) {
            try {
                System.out.print(">");
                String command = userInput.readLine();

                // Vérifier si l'utilisateur veut quitter
                if (command.equals("exit")) {
                    break;
                }

                // Simuler le traitement de la commande
                processCommand(command);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private static void processCommand(String command) {
        // Ici, vous pouvez simuler le traitement de différentes commandes
        // Par exemple, afficher un message pour simuler le téléchargement d'un fichier
        System.out.println("Commande reçue : " + command);
        System.out.println("Traitement de la commande en cours...");
        System.out.println("Commande traitée avec succès !");
    }
}
