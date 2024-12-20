package lammm;

import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.Base64;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Peer {
    private ServeurPair serveur;

    // Constructeur
    public Peer(ServeurPair serveurPair) {
        this.serveur = serveurPair;
    }

    // Connection au Tracker/Pair-Serveur
    public Socket connect(int port) {
        Socket socket = null;
        try {
            socket = new Socket("localhost", port);
            System.out.println("Pair connecté au port :" + port + "\n");
        } catch (Exception e) {
            System.out.println("Le port n'est pas correcte, essayez une nouvelle fois");
            // e.printStackTrace();
        }
        return socket;
    }

    // Déconexion
    public void disconnect(Socket socket) {
        try {
            socket.close();
            System.out.println("déconnexion effectuée avec succées\n");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Socket connectTracker(BufferedReader userInput, Socket socket) {
        int maxAttempts = 3;
        int attempts = 0;
        boolean connected = false;

        while (!connected && attempts < maxAttempts) {
            try {
                System.out.print("Entrez le port du tracker : ");
                int trackerPort = Integer.parseInt(userInput.readLine());
                // socket = this.connect(trackerPort);
                socket = new Socket("localhost", trackerPort);
                connected = true; // Connected successfully
                break;
            } catch (Exception e) {
                attempts++;
                System.out.println("Try again");
            }
        }

        if (socket != null && socket.isConnected()) {
            return socket;
        } else {
            // All attempts failed
            System.out.println("Failed to connect after " + maxAttempts + " attempts. Exiting...");
            System.exit(0); // Terminate the program
            return null;
        }
    }

    public static void main(String[] args) {
        ServeurPair serveur = new ServeurPair();
        Peer peer = new Peer(serveur);
        Socket socket = null;
        PrintWriter out = null;
        BufferedReader in = null;
        BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in));
        // boolean firstConnection = true; // Pour garder une trace de la première
        // connexion

        // int peerPort = findFreePort();
        ServerSocket serverSocket = null;

        Socket peerSocket = null;

        try {
            socket = peer.connectTracker(userInput, socket);

            // Lancement des connections
            peer.serveur.listen();
            peer.serveur.communicateWithPairs();

            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            // peer.serveur.AnnounceUpdate(out, "anounce listen ");
            peer.serveur.updatePeriodically(out);

            System.out.println("> " + in.readLine());

            while (true) {
                try {
                    System.out.print("\n< ");
                    String command = userInput.readLine();

                    if (command.equals("exit")) {
                        System.out.println("You're about to exit the program\n");
                        System.exit(0); // Terminate the program
                        // break;
                    }

                    out.println(command);
                    // Recoit réponse du tracker
                    String response = in.readLine();
                    System.out.println("Réponse du tracker : ");
                    System.out.println("> " + response);

                    if (command.startsWith("getfile")) {
                        // On stocke les ports des ServeurPairs ici
                        List<Integer> ports = new ArrayList<>();

                        String[] words = response.split("\\s+");
                        for (int i = 2; i < words.length - 1; i++) {
                            ports.add(Integer.parseInt(words[i].split(":")[1]));
                        }

                        // Connection parallèle à plusieurs serveurs
                        // Initiation des streams à null pour pouvoir y accéder plus tard
                        List<BufferedReader> inP = new ArrayList<>();
                        List<PrintWriter> outP = new ArrayList<>();

                        for (int i = 0; i < ports.size(); i++) {
                            int peerToConnect = ports.get(i);

                            // Connection au pair Serveur
                            Socket socketPair = peer.connect(peerToConnect);

                            // Initiation des streams
                            try {
                                inP.add(new BufferedReader(new InputStreamReader(socketPair.getInputStream())));
                                outP.add(new PrintWriter(socketPair.getOutputStream(), true));
                            } catch (IOException e) {
                                e.printStackTrace();
                            }

                        }

                        List<String> bMaps = new ArrayList<>(); // Pour stocker les bufferMap
                        String pieceIndex = "";
                        String commande = "";
                        while (true) {
                            System.out.print("< ");
                            commande = userInput.readLine();

                            if (commande.equals("")) {
                                continue;
                            }
                            if (!commande.equals("")) {
                                for (int i = 0; i < outP.size(); i++) {
                                    outP.get(i).println(commande);
                                    outP.get(i).flush();
                                }
                            }

                            if (commande.startsWith("getpieces")) {
                                // On stocke les pieces voulues
                                int startIndex = commande.indexOf("[");
                                int endIndex = commande.indexOf("]");
                                String substring = commande.substring(startIndex + 1, endIndex);

                                // Remove all non-digit characters from the substring
                                pieceIndex = substring.replaceAll("[^0-9]", "");
                            }

                            // Se rappeler d'ajuster les i à l'id du pair
                            for (int i = 0; i < inP.size(); i++) {
                                // if (inP.get(i).ready()) {

                                String responseP = inP.get(i).readLine();
                                if (responseP.startsWith("data")) {
                                    // Lire filekey
                                    // String filekey = responseP.split(" ")[1];
                                    // Decode
                                    Pattern pattern = Pattern.compile("%(.*?)%");
                                    Matcher matcher = pattern.matcher(responseP);

                                    StringBuffer output = new StringBuffer();

                                    int pieceNbr = 0;
                                    while (matcher.find()) {
                                        byte[] decoded = Base64.getDecoder().decode(matcher.group(1));
                                        String data = new String(decoded);

                                        // append to print on terminal
                                        matcher.appendReplacement(output, data);

                                        // Write piece to file

                                        // Create file
                                        File file = new File("./seed/Peer" + peer.serveur.getPeerId() + "/newfile.txt");
                                        if (!file.exists()) {
                                            file.createNewFile();
                                        }
                                        System.out.println("index:" + pieceIndex.charAt(pieceNbr));
                                        int intValue = pieceIndex.charAt(pieceNbr) - '0';
                                        FileWrapper.writePiece(data, intValue,
                                                "./seed/Peer" + peer.serveur.getPeerId() + "/newfile.txt");
                                        peer.serveur.addFile("newfile.txt",
                                                "./seed/Peer" + peer.serveur.getPeerId() + "/newfile.txt");
                                        pieceNbr = pieceNbr + 1;
                                    }
                                    matcher.appendTail(output);
                                    responseP = output.toString();
                                }
                                if (commande.startsWith("interested")) {
                                    // On stocke la bufferMaps
                                    bMaps.add(responseP.split(" ")[2]);

                                }
                                System.out.println("Réponse du pair: " + i);
                                System.out.println("> " + responseP);
                            }
                        }

                    }
                } catch (Exception e) {
                    e.printStackTrace();
                    System.out.println("Connection au pair perdu. Vous etes redirigé vers le tracker.");
                }
            }

        } catch (

        UnknownHostException e) {
            System.err.println("Unknown host: " + "local host");
            System.exit(1);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (socket != null) {
                    peer.disconnect(socket);
                }
                if (peerSocket != null) {
                    peerSocket.close();
                }
                if (serverSocket != null) {
                    serverSocket.close();
                }
                if (out != null) {
                    out.close();
                }
                if (in != null) {
                    in.close();
                }
                if (userInput != null) {
                    userInput.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
