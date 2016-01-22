  if (argc < 4) {
        fprintf(stderr, "Usage: %s %s \n", argv[0], "<Key> <Mirror Directory> <Mount Point>");
        return EXIT_FAILURE;
    }

    struct encfs_state *encfs_data; 
    
    encfs_data = malloc(sizeof(struct encfs_state));

   
    encfs_data->key = argv[argc-3];
    encfs_data->rootdir = realpath(argv[argc-2], NULL);
    argv[argc-3] = argv[argc-1];
    argv[argc-2] = NULL;
    argv[argc-1] = NULL;
    argc = argc - 2;
	return fuse_main(argc, argv, &xmp_oper, encfs_data);
