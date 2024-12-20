package lammm;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.io.*;
import java.util.Base64;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.lang.Math;

public class FileWrapper {
    private static int pieceSize; // Par défaut for now
    private String key;
    private BufferMap bufferMap;
    private String path;
    private File file;

    // Constructor
    public FileWrapper(String fileName, String path) {
        this.path = path; // --includes name
        this.file = new File(path);
        // read pieceSize value from config.ini.example
        try (BufferedReader reader = new BufferedReader(new FileReader("./config.ini.example"))) {
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.startsWith("pieceSize")) {
                    String[] parts = line.split("=");
                    if (parts.length == 2) {
                        FileWrapper.pieceSize = Integer.parseInt(parts[1].trim());
                        break;
                    }
                }
            }
        } catch (IOException | NumberFormatException e) {
            e.printStackTrace();
        }
        this.key = generateKey(); // Generate key automatically
        long size = (long) Math.ceil((double) this.file.length() / pieceSize);
        this.bufferMap = new BufferMap(size); // At the creation of the file, it's complete
    }

    // Getters and setters
    public String getPath() throws IOException {
        return file.getCanonicalPath();
    }

    public String getFileName() {
        return file.getName();
    }

    public long getLength() {
        return file.length();
    }

    public void setPieceSize(int pieceSize) {
        FileWrapper.pieceSize = pieceSize;
    }

    public String getKey() {
        return key;
    }

    public String getBufferMapString() {
        return this.bufferMap.toString();
    }

    public BufferMap getBufferMap() {
        return this.bufferMap;
    }

    // Key generation method using MD5 hashing
    private String generateKey() {
        String key = "";
        try {
            MessageDigest md = MessageDigest.getInstance("MD5");
            md.update(Files.readAllBytes(Paths.get(this.path)));
            byte[] digest = md.digest();
            // Convert bytes to hexadecimal string
            StringBuilder sb = new StringBuilder();
            for (byte b : digest) {
                sb.append(String.format("%02X", b));
            }
            key = sb.toString();
        } catch (NoSuchAlgorithmException | IOException e) {
            e.printStackTrace();
        }
        return key;
    }

    // Check if a piece is available in the buffer map
    public boolean isAvailable(int index) {
        return this.bufferMap.getElement(index) == 1;
    }

    // Read piece
    public void readPeace(PrintWriter out, int pieceIndex) {
        if (isAvailable(pieceIndex)) {
            try {
                // New file object
                File file = new File(this.path);

                FileInputStream fileInputStream = new FileInputStream(file);
                BufferedInputStream bufferedInputStream = new BufferedInputStream(fileInputStream);

                byte[] buffer = new byte[FileWrapper.pieceSize];
                int bytesRead;
                long bytesToSkip = (long) pieceIndex * FileWrapper.pieceSize;

                // Skip bytes corresponding to previous pieces
                long skippedBytes = bufferedInputStream.skip(bytesToSkip);
                if (skippedBytes != bytesToSkip) {
                    bufferedInputStream.close();
                    throw new IOException("Unable to skip to piece " + pieceIndex);
                }

                // Read bytes for the current piece
                bytesRead = bufferedInputStream.read(buffer, 0, FileWrapper.pieceSize);
                if (bytesRead != -1) {
                    // Convert the read bytes to string and send it to the client
                    String data = new String(buffer, 0, bytesRead);
                    // encode base64
                    String encodedString = Base64.getEncoder().encodeToString(data.getBytes());

                    System.out.println(data);
                    out.print(encodedString);
                }

                System.out.println("Download done!\n");

                // close stream
                bufferedInputStream.close();

            } catch (IOException e) {
                e.printStackTrace();
            }
        } else {
            out.println("piece not available");
        }
    }

    public static void writePiece(String data, int index, String path) {
        try {
            // Create a random access file
            RandomAccessFile file = new RandomAccessFile(path, "rw");

            // Calculate the insertion position
            long insertionIndex = FileWrapper.pieceSize * index;

            // Move the file pointer to the insertion position
            file.seek(insertionIndex);

            // Read the data after the insertion point
            byte[] dataAfterInsertion = new byte[(int) (file.length() - insertionIndex)];
            file.read(dataAfterInsertion);

            // Move the file pointer back to the insertion position
            file.seek(insertionIndex);

            // Write the string to insert
            file.write(data.getBytes());

            // Write back the data that was after the insertion point
            file.write(dataAfterInsertion);

            // Close the file
            file.close();

            System.out.println("String inserted successfully at position " + insertionIndex);
        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }

    public String keyToName(String filekey) {
        if (filekey.equals(this.key)) {
            return this.getFileName();
        }
        return "";
    }

    public String formatFileInfo() {
        // Random random = new Random();
        String fileName = this.getFileName().toString();
        // Utilisez Files.size(path) pour obtenir la taille sans avoir besoin de créer
        // un objet File.
        long length = this.getLength();
        // int pieceSize = this.pieceSize; // Exemple de taille de pièce fixe
        // String key = Integer.toHexString(random.nextInt()); // Clé générée de manière
        // simplifiée pour l'exemple
        String key = this.getKey();
        return String.format("%s %d %d %s", fileName, length, FileWrapper.pieceSize, key);
    }

    // toString method to format the output
    @Override
    public String toString() {
        return String.format("%s %d %d %s %s", this.getFileName(), this.getLength(), FileWrapper.pieceSize, key, path);
    }
}
