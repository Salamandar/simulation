
extern "C" {
    void init_hardware();
}
int main(int argc, char const *argv[])
{
    init_hardware();
    return 0;
}
