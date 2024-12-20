package lammm;

public class BufferMap {
    private long size;
    private int[] binaryArray;

    // Constructor
    public BufferMap(long size) {
        this.size = size;
        this.binaryArray = new int[(int) size];
        for (int i = 0; i < size; i++) {
            binaryArray[i] = 1;
        }
    }

    // Getter for size
    public long getSize() {
        return this.size;
    }

    public int getElement(int index) {
        return this.binaryArray[index];
    }

    // Setter
    public void setElementToZero(int index) {
        this.binaryArray[index] = 0;
    }

    public void setElementToOne(int index) {
        this.binaryArray[index] = 1;
    }

    public static BufferMap StringToBufferMap(String bmap) {
        BufferMap b = new BufferMap(bmap.length());
        for (int i = 0; i < bmap.length(); i++) {
            if (bmap.charAt(i) == '1') {
                b.setElementToOne(i);
            } else { // assume string is in the right format
                b.setElementToZero(i);
            }
        }
        return b;
    }

    // toString method to format the output
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < size; i++) {
            sb.append(binaryArray[i]);
        }
        return sb.toString();
    }
}
