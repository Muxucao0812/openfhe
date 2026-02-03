<AutoPilot:project xmlns:AutoPilot="com.autoesl.autopilot.project" projectType="C/C++" name="Solution_ntt" ideType="classic" top="Top">
    <files>
        <file name="src/auto.cpp" sc="0" tb="false" cflags="-I./include -I./src" csimflags="" blackbox="false"/>
        <file name="src/bconv.cpp" sc="0" tb="false" cflags="-I./include -I./src" csimflags="" blackbox="false"/>
        <file name="src/interleave.cpp" sc="0" tb="false" cflags="-I./include -I./src" csimflags="" blackbox="false"/>
        <file name="src/ntt_kernel.cpp" sc="0" tb="false" cflags="-I./include -I./src" csimflags="" blackbox="false"/>
        <file name="src/mod_mult_kernel.cpp" sc="0" tb="false" cflags="-I./include -I./src" csimflags="" blackbox="false"/>
        <file name="src/mod_sub_kernel.cpp" sc="0" tb="false" cflags="-I./include -I./src" csimflags="" blackbox="false"/>
        <file name="src/mod_add_kernel.cpp" sc="0" tb="false" cflags="-I./include -I./src" csimflags="" blackbox="false"/>
        <file name="src/arithmetic.cpp" sc="0" tb="false" cflags="-I./include -I./src" csimflags="" blackbox="false"/>
        <file name="src/load.cpp" sc="0" tb="false" cflags="-I./include -I./src" csimflags="" blackbox="false"/>
        <file name="src/top.cpp" sc="0" tb="false" cflags="-I./include -I./src" csimflags="" blackbox="false"/>
        <file name="../../ntt_intt_test.cpp" sc="0" tb="1" cflags="-I../../include -I../../src -Wno-unknown-pragmas" csimflags="" blackbox="false"/>
    </files>
    <Simulation argv="">
        <SimFlow name="csim" setup="false" optimizeCompile="false" clean="false" ldflags="" mflags=""/>
    </Simulation>
    <solutions>
        <solution name="solution1" status=""/>
    </solutions>
</AutoPilot:project>

