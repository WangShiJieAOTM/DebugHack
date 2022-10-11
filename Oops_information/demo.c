static __init int init_oopsdemo(void) {
    *((int *) 0x00) = 0x19760817;
    return 0;
}

module_init(init_oopsdemo);

static __exit void cleanup_oopsdemo(void){}

module_exit(cleanup_oopsdemo);

MODULE_LICENSE("GPL");