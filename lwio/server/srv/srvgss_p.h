#ifndef __SRVGSS_P_H__
#define __SRVGSS_P_H__

#define BAIL_ON_SEC_ERROR(ulMajorStatus) \
    if ((ulMajorStatus != GSS_S_COMPLETE)\
            && (ulMajorStatus != GSS_S_CONTINUE_NEEDED)) {\
        goto sec_error; \
    }

#define BAIL_ON_KRB_ERROR(ctx, ret)                                   \
    if (ret) {                                                        \
        if (ctx)  {                                                   \
            PCSTR pszKrb5Error = krb5_get_error_message(ctx, ret);    \
            if (pszKrb5Error) {                                       \
                SMB_LOG_ERROR("KRB5 Error at %s:%d: %s",              \
                        __FILE__,                                     \
                        __LINE__,                                     \
                        pszKrb5Error);                                \
                krb5_free_error_message(ctx, pszKrb5Error);           \
            }                                                         \
        } else {                                                      \
            SMB_LOG_ERROR("KRB5 Error at %s:%d [Code:%d]",            \
                    __FILE__,                                         \
                    __LINE__,                                         \
                    ret);                                             \
        }                                                             \
        if (ret == KRB5KDC_ERR_KEY_EXP) {                             \
            ntStatus = SMB_ERROR_PASSWORD_EXPIRED;                    \
        } else if (ret == KRB5_LIBOS_BADPWDMATCH) {                   \
            ntStatus = SMB_ERROR_PASSWORD_MISMATCH;                   \
        } else if (ret == KRB5KRB_AP_ERR_SKEW) {                      \
            ntStatus = SMB_ERROR_CLOCK_SKEW;                          \
        } else if (ret == ENOENT) {                                   \
            ntStatus = SMB_ERROR_KRB5_NO_KEYS_FOUND;                  \
        } else {                                                      \
            ntStatus = SMB_ERROR_KRB5_CALL_FAILED;                    \
        }                                                             \
        goto error;                                                   \
    }

#define SRV_PRINCIPAL       "not_defined_in_RFC4178@please_ignore"
#define SRV_KRB5_CACHE_PATH "MEMORY:lwio_krb5_cc"

typedef struct _SRV_KRB5_CONTEXT
{
    pthread_mutex_t  mutex;
    pthread_mutex_t* pMutex;

    LONG             refcount;

    PSTR            pszCachePath;
    PSTR            pszMachinePrincipal;
    time_t          ticketExpiryTime;

} SRV_KRB5_CONTEXT, *PSRV_KRB5_CONTEXT;

typedef enum
{
    SRV_GSS_CONTEXT_STATE_INITIAL = 0,
    SRV_GSS_CONTEXT_STATE_NEGOTIATE,
    SRV_GSS_CONTEXT_STATE_COMPLETE
} SRV_GSS_CONTEXT_STATE;

typedef struct _SRV_GSS_NEGOTIATE_CONTEXT
{
    SRV_GSS_CONTEXT_STATE state;

    gss_ctx_id_t* pGssContext;

} SRV_GSS_NEGOTIATE_CONTEXT, *PSRV_GSS_NEGOTIATE_CONTEXT;

static
NTSTATUS
SrvGssNewContext(
    PSRV_KRB5_CONTEXT* ppContext
    );

static
NTSTATUS
SrvGssInitNegotiate(
    PSRV_KRB5_CONTEXT          pGssContext,
    PSRV_GSS_NEGOTIATE_CONTEXT pGssNegotiate,
    PBYTE                      pSecurityInputBlob,
    ULONG                      ulSecurityInputBlobLen,
    PBYTE*                     ppSecurityOutputBlob,
    ULONG*                     pulSecurityOutputBloblen
    );

static
NTSTATUS
SrvGssContinueNegotiate(
    PSRV_KRB5_CONTEXT          pGssContext,
    PSRV_GSS_NEGOTIATE_CONTEXT pGssNegotiate,
    PBYTE                      pSecurityInputBlob,
    ULONG                      ulSecurityInputBlobLen,
    PBYTE*                     ppSecurityOutputBlob,
    ULONG*                     pulSecurityOutputBloblen
    );

static
VOID
SrvGssFreeContext(
    PSRV_KRB5_CONTEXT pContext
    );

static
void
srv_display_status(
    PCSTR     pszId,
    OM_uint32 maj_stat,
    OM_uint32 min_stat
    );

static
void
srv_display_status_1(
    PCSTR     pszId,
    OM_uint32 code,
    int       type
    );

static
NTSTATUS
SrvGssRenew(
   PSRV_KRB5_CONTEXT pContext
   );

static
NTSTATUS
SrvGetTGTFromKeytab(
    PCSTR   pszUserName,
    PCSTR   pszPassword,
    PCSTR   pszCachePath,
    time_t* pGoodUntilTime
    );

static
NTSTATUS
SrvDestroyKrb5Cache(
    PCSTR pszCachePath
    );

static
NTSTATUS
SrvSetDefaultKrb5CachePath(
    PCSTR pszCachePath,
    PSTR* ppszOrigCachePath
    );

#endif /* __SRVGSS_P_H__ */
