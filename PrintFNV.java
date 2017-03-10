import java.io.*;

class PrintFNV {

  public static int hashCode(String str) {
    int hash = 2147483647;
    for (int i = 0; i < str.length(); i++) {
      hash = hash * 16777619 ^ str.charAt(i);
    }
    return hash; }

  public static void main(String args[]) {
    try {
      FileInputStream fstream = new FileInputStream("sol.hex");
      DataInputStream in = new DataInputStream(fstream);
      BufferedReader br = new BufferedReader(new InputStreamReader(in));

      String hex;
      while ((hex = br.readLine()) != null)   {
        String str = "";
        for (int i = 0; i < hex.length(); i += 4) {
          int c = Integer.parseInt(hex.substring(i, i+4), 16);
          str += new String(Character.toChars(c));
        }
        System.out.println("Hashcode: " + hex + " " + hashCode(str) );
      }
      in.close();
    }
    catch (Exception e) {
      System.err.println("Error: " + e.getMessage());
    }
  }
}
