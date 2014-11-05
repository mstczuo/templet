import java.io.*;
import java.util.*;
import java.math.*;
import java.text.DecimalFormat;

public class E {
    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        while(in.hasNext()) {
            BigDecimal R = in.nextBigDecimal().stripTrailingZeros();
            int n = in.nextInt();
            String ans = R.pow(n).toPlainString();
            if(ans.startsWith("0.")) {
                ans = ans.substring(1);
            }
            System.out.println(ans);
        }
    }
}
