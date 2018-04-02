/**
 * Created by nagasaty on 11/4/17.
 */
public class RabinKarpHashing {
    
    /**
     * @param c Only ATGC (uppercase) are supported. A=0, T=1, G=2, C=3 ..else returns -1
     * @return returns the index
     */
    public static int getIndex(char c){
        return (c=='A')?0:((c=='T')?1:((c=='G')?2:(c=='C')?3:-1));
    }
    
    public static long hash(String kmer, int sigma){
        long res = 0 ;
        for (char c : kmer.toCharArray()) {
            res = res*sigma + getIndex(c);
        }
        return res;
    }

    public static Integer computeNextHash(int vertexId, int i, int k, int sigma) {
        return vertexId%((int)Math.pow(sigma, k-1)) + i;
    }
    
    public static Integer computePrevHash(int vertexId, int i, int k, int sigma) {
        return i*((int)Math.pow(sigma, k-1)) + vertexId/sigma;
    }

    public static int pivotIndex(int[] nums) {
        int[] forward  = new int[nums.length];
        for(int i = 0 ; i  < nums.length; i++){
            forward[i] = ((i > 0)?(forward[i-1]+nums[i-1]):0);
        }
        for(int i = 0 ; i < nums.length; i++){
            System.out.println(forward[i]);
        }
        int back = 0;
        int res = -1;
        for(int i = nums.length-1; i>=0; i--){
            if(back == forward[i]){
                res = i;
            }
            back = back+nums[i];
        }
        return res;
    }
}
