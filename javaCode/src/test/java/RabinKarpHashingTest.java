import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by nagasaty on 11/4/17.
 */
public class RabinKarpHashingTest {
    RabinKarpHashing obj;
    
    @Before
    public void init(){
        obj = new RabinKarpHashing();
    }
    
    @org.junit.Test
    public void getIndex() throws Exception {
        assertEquals(0, obj.getIndex('A'));
        assertEquals(1, obj.getIndex('T'));
        assertEquals(2, obj.getIndex('G'));
        assertEquals(3, obj.getIndex('C'));
        assertEquals(-1, obj.getIndex('Z'));
    }

    @org.junit.Test
    public void hash() throws Exception {
        assertEquals(27, obj.hash("ATGC", 4));
        assertEquals(27, obj.hash("TGC", 4));
    }

    @Test
    public void computeNextHash() throws Exception {
        int[] arr = {1,2,3};
        System.out.println(RabinKarpHashing.pivotIndex(arr));

    }
}