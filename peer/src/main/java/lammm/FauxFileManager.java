package lammm;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class FauxFileManager {
    public static String getSeedFilesList() {
        // Simuler une liste de fichiers partagés
        List<String> fileList = new ArrayList<>();
        fileList.add("file1.txt");
        fileList.add("file2.txt");
        fileList.add("file3.txt");

        // Construire une chaîne de caractères représentant la liste de fichiers
        StringBuilder fileListString = new StringBuilder("[");
        for (String file : fileList) {
            fileListString.append(file).append(" ");
        }
        fileListString.append("]");

        return fileListString.toString();
    }
}
