function d = string2hash(string)

import java.security.*;
import java.math.*;
import java.lang.String;

md = MessageDigest.getInstance('SHA-1');
hash = md.digest(double(string));
bi = BigInteger(1, hash);
c = char(bi.toString(16));
d = hex2dec(c(33:40));