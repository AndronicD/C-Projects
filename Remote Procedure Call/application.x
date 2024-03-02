struct Request_Auth{
    string clientId<20>;
    int autoRefresh;
};

struct Response_Auth{
    string token<20>;
    string status<50>;
};

struct Request_Approve{
    string authToken<20>;
};

struct Response_Approve{
    int isSigned;
};

struct Request_Bearer{
    string authToken<20>;
    int isSigned;
};

struct Response_Bearer{
    string accessToken<20>;
    string refreshToken<20>;
    string status<50>;
    int timeout;
};

struct Renew_Request_Bearer{
    string refreshToken<20>;
};

struct Renew_Response_Bearer{
    string newAccessToken<20>;
    string newRefreshToken<20>;
    int timeout;
};

struct Request_Validate_Action{
    string action<20>;
    string resource<20>;
    string accessToken<30>;
};

struct Response_Validate_Action{
    string status<50>;
};

program MANAGER_OF_RESOURCES_APP {
    version MANAGER_OF_RESOURCES_APP_VERSION {
        Response_Auth AUTHENTICATE(Request_Auth) = 1;
        Response_Approve APPROVE(Request_Approve) = 2;
        Response_Bearer BEARER(Request_Bearer) = 3;
        Renew_Response_Bearer RENEW_BEARER(Renew_Request_Bearer) = 4;
        Response_Validate_Action VALIDATE_ACTION(Request_Validate_Action) = 5;
    } = 1;
} = 0x12345678;