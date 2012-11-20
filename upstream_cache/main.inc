	server_name	10.0.0.4;

	keepalive_timeout 20;

	access_log  off;
	root	/var/empty;

	location = /live.txt {
		expires -1;
		alias	/var/empty/live.txt;
	}

	expires -1;

	# strip unneeded GET parameters
	if ($args ~ (.*)rev_n=[^&]*(.*)) {
                set $args $1$2;
        }
        # cleanup any repeated & introduced
        if ($args ~ (.*)&&+(.*)) {
                set $args $1&$2;
        }
        # cleanup leading &
        if ($args ~ ^&(.*)) {
                set $args $1;
        }
        # cleanup ending &
        if ($args ~ (.*)&$) {
                set $args $1;
        }


	location ~ / {
		access_log	  off;
		error_log /var/log/nginx/proxy.log;
		proxy_pass http://upstream.static.site.net;
		proxy_cache static;
		proxy_cache_key $uri$is_args$args;
		#proxy_cache_key $request_uri;
		proxy_cache_valid  30d;
		expires 864000;
		set $do_not_cache 0;

		if ($request_uri ~ rand=){
			set $do_not_cache 1;
			access_log      /var/log/nginx/nocache.log;
		}

		proxy_no_cache $do_not_cache;
		proxy_cache_bypass $do_not_cache;
	}
