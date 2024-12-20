package lammm;

import java.io.*;
import java.net.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.*;

public class ServeurPair {
    private int maxConnections = 5; // By default 5
    private ServerSocket serverSocket;
    private List<FileWrapper> files = new ArrayList<>(); // Stockage des fichiers owned par le pair
    private FileManager fileManager;
    private int peerId;
    private final ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);
    // private int isDownloading = 0; // For leech --- download

    // Initiation de la liste des fichiers ?

    public ServeurPair() {
        Random random = new Random();
        int id = random.nextInt(3) + 1;
        this.peerId = id;
        this.serverSocket = null;
        String path = "./seed/Peer" + this.peerId + "/";

        this.fileManager = new FileManager(path);

        // Other initialization code...

        try {
            String fileList = this.fileManager.getSeedFilesList();
            fileList = fileList.substring(1, fileList.length() - 1); // remove brackets
            String[] filesInfo = fileList.split("\\s+");
            int i = 0;
            while (i < filesInfo.length) {
                String pathFile = path + filesInfo[i];
                try {
                    // Create fileWrapper object with info
                    FileWrapper file = new FileWrapper(filesInfo[i], pathFile);
                    // Add to ArrayList
                    this.files.add(file);
                } catch (NumberFormatException e) {
                    e.printStackTrace();
                }
                i = i + 4;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public Integer getPeerId() {
        return this.peerId;
    }

    public String AnnounceUpdate(PrintWriter out, String state) {
        String announceMessage = "";
        try {
            String fileList = this.fileManager.getSeedFilesList();
            announceMessage = state + this.serverSocket.getLocalPort() + " seed " + fileList
                    + " leech [] \n";
            out.println(announceMessage);
            System.out.println("\n< " + announceMessage);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return announceMessage;
    }

    // anounce message sent every 30 seconds
    public void updatePeriodically(PrintWriter out) {
        scheduler.scheduleAtFixedRate(() -> AnnounceUpdate(out, "announce listen "), 0, 120, TimeUnit.SECONDS);
    }

    public void listen() {
        try {
            serverSocket = new ServerSocket(0);
            int port = serverSocket.getLocalPort();
            System.out.println("Le pair est en écoute sur le port :" + port + "\n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void communicateWithPairs() {
        ExecutorService executor = Executors.newFixedThreadPool(maxConnections);
        System.out.println("Server is listening on port " + serverSocket.getLocalPort() + "\n");

        try {
            executor.submit(() -> {
                while (true) {
                    try {
                        Socket socket = serverSocket.accept();
                        System.out.println("Server accepted connection\n");
                        ConversationHandler clientsock = new ConversationHandler(socket, this.files);
                        new Thread(clientsock).start();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            });
        } finally {
            executor.shutdown();
            // shutServer(serverSocket);
        }
    }

    public Boolean hasFile(String filekey) {
        for (FileWrapper file : this.files) {
            if (file.getKey().equals(filekey)) {
                return true;
            }
        }
        return false;
    }

    public void addFile(String filename, String path) {
        FileWrapper newFile = new FileWrapper(filename, path);
        this.files.add(newFile);
    }

    public static void shutServer(ServerSocket serverSocket) {
        try {
            if (serverSocket != null && !serverSocket.isClosed()) {
                serverSocket.close();
                System.out.println("Server socket closed");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    static class ConversationHandler implements Runnable {
        private Socket socket;
        private List<FileWrapper> files;

        ConversationHandler(Socket socket, List<FileWrapper> f) {
            this.socket = socket;
            this.files = f;
        }

        @Override
        public void run() {
            PrintWriter outPeer = null;
            BufferedReader inPeer = null;
            try {
                outPeer = new PrintWriter(socket.getOutputStream(), true);
                inPeer = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                String commandStr = "";
                while (true) {
                    int maxAttempts = 3;
                    int attempts = 0;
                    boolean isValid = false;

                    // outPeer.print(">");
                    System.out.println("\n>");
                    command Command = null;
                    while (attempts < maxAttempts && !isValid) {
                        if (inPeer.ready()) {
                            commandStr = inPeer.readLine();
                            System.out.println(commandStr);

                            if (commandStr.startsWith("interested")) {
                                Command = new interested(commandStr);
                                isValid = Command.isValid(commandStr);
                            } else if (commandStr.startsWith("getpieces")) {
                                Command = new getpieces(commandStr);
                                isValid = Command.isValid(commandStr);
                            } else if (commandStr.equals("exit")) {
                                break;
                            }
                            if (!isValid) {
                                attempts++;
                                outPeer.println("Commande invalide, essayez une nouvelle fois");
                            }
                        }
                    }
                    if (isValid) {
                        Command.execute(commandStr, files, outPeer);
                    } else {
                        break;
                    }
                }
            } catch (

            IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    if (outPeer != null) {
                        outPeer.close();
                    }
                    if (inPeer != null) {
                        inPeer.close();
                        outPeer.println("Vous allez etre déconecté");
                        socket.close();
                        System.out.println("Peer disconnected succesfully\n");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
