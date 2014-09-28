import java.util.*;
import java.io.*;
import java.math.*;

class A {
    public static void main(String args[]) {
        BufferedReader bufr = new BufferedReader(new InputStreamReader(System.in));
        BufferedWriter bufw = new BufferedWriter(new OutputStreamWriter(System.out));
        String line = null;
        try{
            int n = Integer.parseInt(bufr.readLine());
            for(int i = 1; i <= n; ++i) {
                BigInteger m = new BigInteger(bufr.readLine());
                BigInteger t = m.multiply(m.subtract(BigInteger.valueOf(1))).multiply(BigInteger.valueOf(8));
                m = m.add(BigInteger.valueOf(1)).add(t);
                bufw.write("Case #");
                bufw.write(Integer.toString(i));
                bufw.write(": ");
                bufw.write(m.toString());
                bufw.newLine();
                bufw.flush();
            }
        }catch(IOException e) {
            throw new RuntimeException("IO ERROR");
        }
        try{
            bufr.close();
            bufw.close();
        }catch(Exception e) {
            throw new RuntimeException("IO ERROR");
        }
    }
}
